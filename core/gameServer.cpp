#include "gameServer.hpp"

GameServer::GameServer(uint16_t port) : workGuard(asio::make_work_guard(ioContext)), socket(ioContext, udp::endpoint(udp::v4(), port)), running(false)
{
    InitPhysics();
}
GameServer::~GameServer()
{
    running = false;
    ioContext.stop();
}

void GameServer::Run()
{
    running = true;
    StartReceive();

    using tickRate = std::chrono::duration<uint32_t, std::ratio<1, 60>>;

    auto nextTick = std::chrono::high_resolution_clock::now();

    std::cout << "Server started at tickrate: 60 ticks per second" << std::endl;

    while (running)
    {
        ioContext.poll();

        if (ioContext.stopped())
        {
            ioContext.restart();
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (currentTime >= nextTick)
        {
            Tick();
            nextTick += std::chrono::nanoseconds(1000000000 / 60);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
void GameServer::Tick()
{

    serverTickCount++;

    // dynamicsWorld->stepSimulation(1.0 / 60.0f, 1, 1.0 / 60.0f);

    // for (auto& [id, client] : clients) {

    // }
    // if (serverTickCount % 3 == 0)
    // {

    //     Packet snapshot;
    //     snapshot.Write(PacketType::SNAPSHOT);
    //     snapshot.Write((uint32_t)clients.size());

    //     for (auto const &[id, client] : clients)
    //     {

    //         snapshot.Write(client.id);
    //         snapshot.Write(client.state.position);
    //         snapshot.Write(client.state.lastProcessedInput);
    //     }

    //     Broadcast(snapshot);
    // }
}
void GameServer::Broadcast(const Packet &packet)
{
    for (auto const &[id, client] : clients)
    {
        Send(client.endpoint, packet);
    }
}

void GameServer::InitPhysics()
{
    collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();

    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());

    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    dynamicsWorld->getPairCache()->setInternalGhostPairCallback(&ghostPairCallback);
}

void GameServer::Send(const udp::endpoint &endpoint, const Packet &packet)
{
    auto data = std::make_shared<std::vector<uint8_t>>(packet.data);
    socket.async_send_to(asio::buffer(*data), remoteEndpoint, [data](const std::error_code &ec, std::size_t bytesSent) {});
}

void GameServer::StartReceive()
{
    socket.async_receive_from(asio::buffer(recvBuffer), remoteEndpoint, [this](const asio::error_code &ec, std::size_t bytesReceived)
                              {
        if (!ec && bytesReceived > 0) {
            Packet packet;
            packet.data.assign(recvBuffer, recvBuffer + bytesReceived);
            HandleReceive(packet);
        }
        if (running) {
            StartReceive();
        } });
}
void GameServer::HandleReceive(Packet &packet)
{
    PacketType packetType;
    if (!packet.Read(packetType))
        return;

    switch (packetType)
    {
    case PacketType::PING:
    {
        Packet response;
        response.Write(PacketType::PING);
        response.WriteString("Hello from server!");
        Send(remoteEndpoint, response);
        break;
    }
    }
}
