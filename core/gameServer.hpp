#pragma once

#include <iostream>
#include <cstdint>
#include <map>
#include <thread>
#include <chrono>

#include <asio.hpp>

// #include <btBulletDynamicsCommon.h>
// #include <BulletDynamics/Character/btKinematicCharacterController.h>
// #include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "packet.hpp"
#include "network.hpp"

using asio::ip::udp;
using namespace tag::network;

struct PlayerState
{
    uint32_t id;
    glm::vec3 position;
    uint32_t lastProcessedInput; // For reconciliation
};

struct RemoteClient
{
    uint32_t id;
    udp::endpoint endpoint;
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
    void HandleReceive(Packet &packet, udp::endpoint& endpoint);
    void InitPhysics();

    asio::io_context ioContext;
    asio::executor_work_guard<asio::io_context::executor_type> workGuard;
    udp::socket socket;
    udp::endpoint remoteEndpoint;

    uint8_t recvBuffer[2048];
    std::thread serverThread;
    std::mutex clientsMutex;
    std::unordered_map<uint32_t, RemoteClient> clients;
    std::unordered_map<uint32_t, PlayerState> states;
    uint32_t serverTickCount = 0;

    bool running = false;
    uint32_t nextClientId = 1;

    // std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    // std::unique_ptr<btCollisionDispatcher> dispatcher;
    // std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    // std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    // std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

    // // Physics Helpers
    // btGhostPairCallback ghostPairCallback;

    const double tickRate = 1.0 / 60.0;
};
