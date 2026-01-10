#pragma once

#include <string>
#include <thread>
#include <cstdint>
#include <asio.hpp>

#include "packet.hpp"
#include "types.hpp"

#include "serverPlayer.hpp"
#include "serverScene.hpp"

using asio::ip::udp;

class GameServer {
    public:
        GameServer(uint16_t port);
        ~GameServer();

        void Run();
        void Stop();

    private:
        void handleReceive();
        void handlePhysics();
        void broadcastSnapshots();

        void handlePacket(const Packet& packet, const udp::endpoint& endpoint);
        void handleInputPacket(const Packet& packet, uint32_t playerId);
        void sendPacket(const Packet& packet, const udp::endpoint& endpoint);

        uint32_t createPlayer(const udp::endpoint& endpoint);
        void remotePlayer(uint32_t id);

        asio::io_context ioContext;
        udp::socket socket;

        std::thread serverThread;
        std::thread physicsThread;
        
        bool running = true;

        std::unique_ptr<ServerScene> scene;
        std::unordered_map<uint32_t, std::unique_ptr<ServerPlayer>> players;
        std::unordered_map<uint32_t, udp::endpoint> endpoints;

        float fixedDelta = 1.0f / 60.0f;
        uint32_t nextId = 1;
};