#pragma once

#include <iostream>
#include <cstdint>
#include <map>

#include <asio.hpp>

#include "player.hpp"

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

        std::thread serverThread;
        asio::io_context ioContext;
        asio::executor_work_guard<asio::io_context::executor_type> workGuard;
        udp::socket socket;
        udp::endpoint remoteEndpoint;
        uint8_t receiveBuffer[2048];

        std::thread physicsThread;
        std::unique_ptr<PhysicsScene> scene;

        std::unordered_map<uint32_t, udp::endpoint> endpoints;
        std::unordered_map<uint32_t, std::unique_ptr<Player>> players;


        bool running = false;
        uint32_t nextPlayerId = 1;

        const double TICK_RATE = 1.0 / 60.0;
}

// class GameServer
// {
// public:
//     GameServer(uint16_t port);
//     ~GameServer();

//     void Run();

//     void Broadcast(const Packet& packet);   

//     void Send(const Packet& packet, const udp::endpoint& endpoint);

// private:
//     void StartReceive();
//     void HandleReceive(Packet& packet, const udp::endpoint& endpoint);
    
//     void HandleInput(Packet& packet);
//     void HandleJoin(Packet& packet, const udp::endpoint& endpoint);
//     void HandleLeave(Packet& packet, const udp::endpoint& endpoint);

//     asio::io_context ioContext;
//     asio::executor_work_guard<asio::io_context::executor_type> workGuard;
//     udp::socket socket;
//     udp::endpoint remoteEndpoint;
//     uint8_t recvBuffer[2048];

//     std::thread serverThread;

//     std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
//     std::unordered_map<uint32_t, udp::endpoint> endpoints;

//     bool running = false;
//     uint32_t nextId = 1;

//     const double tickRate = 1.0 / 60.0;
// };
