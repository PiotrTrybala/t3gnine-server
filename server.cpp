#include "server.hpp"

GameServer::GameServer(uint16_t port) : workGuard(asio::make_work_guard(ioContext)), socket(ioContext, udp::endpoint(udp::v4(), port))
{
}
GameServer::~GameServer()
{
    ioContext.stop();
    if (serverThread.joinable())
    {
        serverThread.join();
    }
}

void GameServer::Run()
{
    StartReceive();
    serverThread = std::thread([this]
                               { std::cout << "Network thread started..." << std::endl;
            try {
                ioContext.run();
            } catch (const std::exception& e) {
                std::cerr << "Network thread error: " << e.what() << std::endl;
            }
            std::cout << "Network thread stopped." << std::endl; });
}

void GameServer::Broadcast(const Packet &packet)
{
    for (auto &[id, endpoint] : endpoints)
    {
        Send(packet, endpoint);
    }
}

void GameServer::Send(const Packet &packet, const udp::endpoint &endpoint)
{
    socket.async_send_to(asio::buffer(packet.data), endpoint, [this](const std::error_code &ec, std::size_t length)
                         { std::cout << "Send packet with length of " << length << " bytes" << std::endl; });
}

void GameServer::StartReceive()
{
    udp::endpoint sender;

    socket.async_receive_from(
        asio::buffer(recvBuffer), sender, [this, sender](const asio::error_code &ec, std::size_t bytesReceived)
        {
            std::cout << "Got packet from: " << sender.address().to_string() << ":" << sender.port() << std::endl;
            if (!ec && bytesReceived > 0) {
                Packet packet;
                packet.data.assign(recvBuffer, recvBuffer + bytesReceived);

                HandleReceive(packet, sender);
            }
            StartReceive(); });
}
void GameServer::HandleReceive(Packet &packet, const udp::endpoint& endpoint)
{
    PacketType packetType;
    if (!packet.Read(packetType))
    {
        std::cout << "Could not read packet type" << std::endl;
        return;
    }
    Packet response;

    switch (packetType)
    {
    case PacketType::Ping:
        response.Write(PacketType::Ping);
        response.WriteString("Hello from server! :)");
        break;
    case PacketType::PlayerInput:
    {
        HandleInput(packet);
        return;
    }
    case PacketType::PlayerJoin:
    {
        HandleJoin(packet, endpoint);
        return;
    }
    case PacketType::PlayerLeave:
    {
        HandleLeave(packet, endpoint);
        return;
    }
    }
    Send(response, remoteEndpoint);
}

void GameServer::HandleInput(Packet &packet)
{

    // read input

    PlayerInput input;

    if (!packet.Read(input.id))
        return;
    
    if (!packet.Read(input.sequence))
        return;

    if (!packet.Read(input.front.x))
        return;

    if (!packet.Read(input.front.y))
        return;

    if (!packet.Read(input.front.z))
        return;

    if (!packet.Read(input.directionMask))
        return;

    if (!packet.Read(input.isJumping))
        return;

    players[input.id]->ApplyInput(input);
}
void GameServer::HandleJoin(Packet &packet, const udp::endpoint& endpoint)
{

    // add new controller to scene

    uint32_t id = nextId++;

    players[id] = std::make_unique<Player>(id);
    endpoints[id] = endpoint;

    Packet response;
    response.Write(PacketType::PlayerJoined);
    response.Write(id);

    // broadcast new player to client
    Send(response, endpoint);
}
void GameServer::HandleLeave(Packet &packet, const udp::endpoint& endpoint)
{
    // remove controller from scene
    uint32_t id;
    if (!packet.Read(id)) return;

    players.erase(id);
    endpoints.erase(id);

    // broadcast player has left to clients
    Packet response;
    response.Write(PacketType::PlayerLeft);
    response.Write(id);

    Send(response, endpoint);
}