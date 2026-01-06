#include "gameServer.hpp"

GameServer::GameServer(uint16_t port) : workGuard(asio::make_work_guard(ioContext)), socket(ioContext, udp::endpoint(udp::v4(), port))
{
    std::cout << "Created server on port " << port << "..." << std::endl;
}
GameServer::~GameServer()
{
    ioContext.stop();
    if (serverThread.joinable())
        serverThread.join();
}

void GameServer::Run()
{
    startReceive();
    serverThread = std::thread([this]
                               {
        std::cout << "Started listening..." << std::endl;
        ioContext.run(); });
}

void GameServer::Broadcast(const Packet &packet)
{
}

void GameServer::Send(const udp::endpoint &endpoint, const Packet &packet)
{
    socket.async_send_to(asio::buffer(packet.data), endpoint, [this](const std::error_code &ec, std::size_t bytesReceived)
                         { std::cout << "Sent packet with size of " << bytesReceived << " bytes" << std::endl; });
}

void GameServer::startReceive()
{

    socket.async_receive_from(
        asio::buffer(recvBuffer), endpoint, [this](const std::error_code &ec, size_t bytesReceived)
        {
            std::cout << "Received packet from: " << endpoint.address() << ":" << endpoint.port() << std::endl;

            if (!ec && bytesReceived > 0) {
            Packet packet;
            packet.data.assign(recvBuffer, recvBuffer + bytesReceived);

            handleReceive(packet);
            }

            startReceive(); });
}

void GameServer::handleReceive(Packet &packet)
{
    PacketType packetType;

    if (!packet.Read(packetType))
    {
        std::cout << "Could not read packet type from packet" << std::endl;
        return;
    }

    Packet response;

    switch (packetType)
    {

    case PacketType::LOGIN_PLAIN_TEXT:
    {
        std::string username;
        if (!packet.ReadString(username))
        {
            std::cout << "Could not read username from packet" << std::endl;
            return;
        }

        nextId++;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients[nextId] = {
                .endpoint = endpoint,
                .id = nextId,
                .username = username,
            };
        }

        response.Write(PacketType::LOGIN_PLAIN_TEXT_ACK);
        response.Write(nextId);

        break;
    }

    case PacketType::PING:
    {
        uint32_t id;
        if (!packet.Read(id))
        {
            response.Write(PacketType::PING);
            response.WriteString("Invalid clientId");
            break;
        }

        auto it = clients.find(id);
        if (it == clients.end())
        {
            response.Write(PacketType::PING);
            response.WriteString("Client was this id was not found");
            break;
        }

        response.Write(PacketType::PING);
        response.Write(id);
        break;
    }
    }

    Send(endpoint, response);
}