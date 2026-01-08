#pragma once

#include <unordered_map>
#include <memory>
#include <thread>
#include <atomic>
#include <cstdint>
#include <asio.hpp>

#include "network.hpp"
#include "packet.hpp"
#include "serverPlayer.hpp"
#include "gameScene.hpp"

class GameServer
{
public:
    GameServer(uint16_t port);
    ~GameServer();

    void Start();
    void Stop();

private:
    void handleReceive();
    void handlePhysics();
    void boradcastSnapshots();

    void handlePacket(const tag::network::Packet &packet, const asio::ip::udp::endpoint &endpoint);
    void handleInputPacket(const tag::network::Packet &packet, uint32_t playerId);
    void sendPacket(const tag::network::Packet &packet, const asio::ip::udp::endpoint &endpoint);

    uint32_t createPlayer(const asio::ip::udp::endpoint &endpoint);
    void remotePlayer(uint32_t id);

    asio::io_context ioContext;
    asio::ip::udp::socket socket;
    std::thread netThread;
    std::thread physicsThread;
    std::atomic<bool> running;

    std::unique_ptr<GameScene> scene;
    std::unordered_map<uint32_t, std::unique_ptr<ServerPlayer>> players;
    std::unordered_map<uint32_t, asio::ip::udp::endpoint> playerEndpoints;

    float fixedDelta = 1.f / 60.f;
    uint32_t nextId = 1;
};