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

void GameServer::StartReceive()
{
    socket.async_receive_from(
        asio::buffer(recvBuffer), remoteEndpoint, [this](const asio::error_code &ec, std::size_t bytesReceived)
        {
            std::cout << "Got packet from: " << remoteEndpoint.address().to_string() << ":" << remoteEndpoint.port() << std::endl;
            if (!ec && bytesReceived > 0) {

                std::cout << "Received buffer: " << recvBuffer << std::endl;
        
                PacketBuffer packet;
                packet.data.assign(recvBuffer, recvBuffer + bytesReceived);

                std::string message; int value;
                packet.ReadString(message);
                packet.Read(value);

                std::cout << "Got from client: " << message << " " << value << std::endl;    

                HandleReceive(bytesReceived);
            }
            StartReceive(); });
}
void GameServer::HandleReceive(std::size_t length)
{
    PacketBuffer packet;
    packet.WriteString("Hello from server");
    packet.Write(10);

    socket.async_send_to(asio::buffer(packet.data), remoteEndpoint, [packet](const asio::error_code &ec, std::size_t length) {

    });
}