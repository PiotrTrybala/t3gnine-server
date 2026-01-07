#pragma once

#include <iostream>
#include <cstdint>
#include <map>
#include <thread>
#include <chrono>
#include <asio.hpp>


#include "packet.hpp"
#include "network.hpp"

using asio::ip::udp;
using namespace tag::lib::network;

struct RemoteClient
{
    udp::endpoint endpoint;
    uint32_t id;
};

class GameServer
{
public:
    GameServer(uint16_t port);
    ~GameServer();

    void Run();
    void Tick();
    void Broadcast(const Packet &packet);

    void Send(const udp::endpoint &endpoint, const Packet &packet);

private:
    void StartReceive();
    void HandleReceive(Packet &packet);

    asio::io_context ioContext;
    asio::executor_work_guard<asio::io_context::executor_type> workGuard;
    udp::socket socket;
    udp::endpoint remoteEndpoint;

    uint8_t recvBuffer[2048];
    std::thread serverThread;
    std::mutex clientsMutex;
    std::unordered_map<uint32_t, RemoteClient> clients;

    bool running = false;
    uint32_t nextClientId = 1;

    const double tickRate = 1.0 / 60.0;
};
