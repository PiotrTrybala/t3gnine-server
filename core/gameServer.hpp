#pragma once

#include <iostream>
#include <cstdint>
#include <map>
#include <asio.hpp>

#include "packetBuffer.hpp"

using asio::ip::udp;

class GameServer
{
public:
    GameServer(uint16_t port);
    ~GameServer();

    void Run();

private:
    void StartReceive();
    void HandleReceive(std::size_t length);

    asio::io_context ioContext;
    asio::executor_work_guard<asio::io_context::executor_type> workGuard;
    udp::socket socket;
    udp::endpoint remoteEndpoint;
    uint8_t recvBuffer[2048];

    std::thread serverThread;

    // std::map<uint32_t, udp::endpoint> clients;

    // bool running = false;
    // uint32_t nextId = 1;

    // const double tickRate = 1.0 / 60.0;
};
