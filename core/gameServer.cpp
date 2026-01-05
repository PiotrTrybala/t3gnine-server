#include "gameServer.hpp"

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
}
void GameServer::SendTo(const Packet &packet, const Client &client)
{
}

void GameServer::StartReceive()
{
    socket.async_receive_from(
        asio::buffer(recvBuffer), remoteEndpoint, [this](const asio::error_code &ec, std::size_t bytesReceived)
        {
            std::cout << "Got packet from: " << remoteEndpoint.address().to_string() << ":" << remoteEndpoint.port() << std::endl;
            if (!ec && bytesReceived > 0) {

                std::cout << "Received buffer: " << recvBuffer << std::endl;
        
                Packet packet;
                packet.data.assign(recvBuffer, recvBuffer + bytesReceived);

                std::string message; int value;
                packet.ReadString(message);
                packet.Read(value);

                std::cout << "Got from client: " << message << " " << value << std::endl;    

                HandleReceive(packet);
            }
            StartReceive(); });
}
void GameServer::HandleReceive(Packet &packet)
{
    PacketType packetType;
    if (!packet.Read(packetType))
    {
        std::cout << "Could not read packet type" << std::endl;
        return;
    }

    switch (packetType)
    {
    case PacketType::CONNECT:
        break;
    case PacketType::PING:
        break;
    }

    socket.async_send_to(asio::buffer(packet.data), remoteEndpoint, [packet](const asio::error_code &ec, std::size_t length) {});
}