#include "gameServer.hpp"

GameServer::GameServer(uint16_t port) : socket(ioContext, udp::endpoint(udp::v4(), port))
{
    StartReceive();

    serverThread = std::thread([this, port]
                               {
                                std::cout << "Staring server on port " << port << "..." << std::endl; 
                                ioContext.run(); });
}
GameServer::~GameServer()
{
    ioContext.stop();
    if (serverThread.joinable())
    {
        serverThread.join();
    }
}

void GameServer::StartReceive()
{
    socket.async_receive_from(
        asio::buffer(recvBuffer), remoteEndpoint, [this](const asio::error_code& ec, std::size_t bytesReceived){
            std::cout << "Got connection from: " << remoteEndpoint.address().to_string() << ":" << remoteEndpoint.port() << std::endl;
            if (!ec && bytesReceived > 0) {
                HandleReceive(bytesReceived);
            }
            StartReceive();
        });
}
void GameServer::HandleReceive(std::size_t length)
{
    PacketBuffer packet;
    packet.Write("Hello from server");
    packet.Write(10);

    socket.async_send_to(asio::buffer(packet.data), remoteEndpoint, [packet](const asio::error_code& ec, std::size_t length){

    });
}