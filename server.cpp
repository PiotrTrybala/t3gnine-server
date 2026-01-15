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

    if (physicsThread.joinable())
    {
        physicsThread.join();
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

void GameServer::Send(const Packet &packet, const udp::endpoint &endpoint)
{
}

void GameServer::Tick() {
    
}

void GameServer::StartReceive()
{
}

void GameServer::HandleReceive(Packet &packet, const udp::endpoint &endpoint)
{
}