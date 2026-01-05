#pragma once

#include <iostream>
#include <cstdint>
#include <map>
#include <asio.hpp>

#include "packet.hpp"
#include "network.hpp"
#include "client.hpp"

using asio::ip::udp;
using namespace tag::lib::network;

class GameServer
{
public:
    GameServer(uint16_t port);
    ~GameServer();

    void Run();

    void Broadcast(const Packet& packet);   

    void Send(const Packet& packet, const udp::endpoint& endpoint);
    void SendClient(const Packet& packet, const Client& client);

private:
    void StartReceive();
    void HandleReceive(Packet& packet);

    asio::io_context ioContext;
    asio::executor_work_guard<asio::io_context::executor_type> workGuard;
    udp::socket socket;
    udp::endpoint remoteEndpoint;
    uint8_t recvBuffer[2048];

    std::thread serverThread;

    std::map<uint32_t, Client> clients;

    bool running = false;
    uint32_t nextId = 1;

    const double tickRate = 1.0 / 60.0;
};
