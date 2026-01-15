#pragma once

#include <iostream>
#include <cstdint>
#include <map>
#include <memory>
#include <thread>
#include <unordered_map>
#include <chrono>

#include <asio.hpp>

#include "player.hpp"
#include "scene.hpp"

#include "packet.hpp"
#include "types.hpp"

using asio::ip::udp;

class GameServer {
    public:
        GameServer(uint16_t port);
        ~GameServer();

        void Run();

        void Broadcast(const Packet& packet);

        void Send(const Packet& packet, const udp::endpoint& endpoint);

    private:
        void StartReceive();
        void HandleReceive(Packet& packet, const udp::endpoint& endpoint);
        void Tick();

        std::thread serverThread;
        asio::io_context ioContext;
        asio::steady_timer tickTimer;
        asio::executor_work_guard<asio::io_context::executor_type> workGuard;
        udp::socket socket;
        udp::endpoint remoteEndpoint;
        uint8_t receiveBuffer[2048];
        std::unique_ptr<PhysicsScene> scene;

        std::unordered_map<uint32_t, udp::endpoint> endpoints;
        std::unordered_map<uint32_t, std::unique_ptr<Player>> players;

        bool running = false;
        uint32_t nextPlayerId = 1;

        const double TICK_RATE = 1.0 / 60.0;
};