#include "gameServer.hpp"

GameServer::GameServer(uint16_t port) : socket(ioContext, udp::endpoint(udp::v4(), port)), running(false)
{
}
GameServer::~GameServer()
{
    Stop();
}

void GameServer::Start()
{
    running = true;
    netThread = std::thread([this] { handleReceive(); });
    physicsThread = std::thread([this] { handlePhysics(); });
}
void GameServer::Stop()
{
    running = false;
    ioContext.stop();

    if (netThread.joinable()) netThread.join();
    if (physicsThread.joinable()) physicsThread.join();
}

void GameServer::handleReceive()
{
}
void GameServer::handlePhysics()
{
}
void GameServer::broadcastSnapshots()
{
}

void GameServer::handlePacket(const tag::network::Packet &packet, const asio::ip::udp::endpoint &endpoint)
{
}
void GameServer::handleInputPacket(const tag::network::Packet &packet, uint32_t playerId)
{
}
void GameServer::sendPacket(const tag::network::Packet &packet, const asio::ip::udp::endpoint &endpoint)
{
}

uint32_t GameServer::createPlayer(const asio::ip::udp::endpoint &endpoint)
{
    uint32_t id = nextId++;


    return id;
}
void GameServer::remotePlayer(uint32_t id)
{
}