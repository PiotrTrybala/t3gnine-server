// #pragma once

// #include <unordered_map>
// #include <memory>
// #include <thread>
// #include <atomic>
// #include <cstdint>
// #include <asio.hpp>

// #include "network.hpp"
// #include "packet.hpp"
// #include "serverPlayer.hpp"
// #include "gameScene.hpp"

// using asio::ip::udp;

// class GameServer
// {
// public:
//     GameServer(uint16_t port);
//     ~GameServer();

//     void Start();
//     void Stop();

// private:
//     void handleReceive();
//     void handlePhysics();
//     void broadcastSnapshots();

//     void handlePacket(const tag::network::Packet &packet, const asio::ip::udp::endpoint &endpoint);
//     void handleInputPacket(const tag::network::Packet &packet, uint32_t playerId);
//     void sendPacket(const tag::network::Packet &packet, const asio::ip::udp::endpoint &endpoint);

//     uint32_t createPlayer(const asio::ip::udp::endpoint &endpoint);
//     void remotePlayer(uint32_t id);
//     asio::io_context ioContext;
//     asio::ip::udp::socket socket;
//     std::thread netThread;
//     std::thread physicsThread;
//     std::atomic<bool> running;

//     std::unique_ptr<GameScene> scene;
//     std::unordered_map<uint32_t, std::unique_ptr<ServerPlayer>> players;
//     std::unordered_map<uint32_t, asio::ip::udp::endpoint> playerEndpoints;

//     float fixedDelta = 1.f / 60.f;
//     uint32_t nextId = 1;
// };

// #pragma once

// #pragma once
// #include <unordered_map>
// #include <memory>
// #include <mutex>

// #include <btBulletDynamicsCommon.h>
// #include "serverPlayer.hpp"

// class GameScene
// {
// public:
//     GameScene();
//     ~GameScene();

//     void Step(float deltaTime);

//     uint32_t AddRigidBody(btRigidBody *body);
//     void RemoteRigidBody(uint32_t id);

//     uint32_t AddCollider(btCollisionObject *object);
//     void RemoveCollider(uint32_t id);

//     void AddPlayer(std::shared_ptr<ServerPlayer> player);
//     void RemovePlayer(uint32_t id);

//     std::shared_ptr<ServerPlayer> GetPlayer(uint32_t id);

// private:
//     btDefaultCollisionConfiguration *collisionConfig;
//     btCollisionDispatcher *dispatcher;
//     btBroadphaseInterface *broadphase;
//     btSequentialImpulseConstraintSolver *solver;
//     btDiscreteDynamicsWorld *world;

//     std::unordered_map<uint32_t, btCollisionObject*> colliders;
//     std::unordered_map<uint32_t, btRigidBody *> rigidBodies;
//     std::unordered_map<uint32_t, std::shared_ptr<ServerPlayer>> players;

//     uint32_t nextID = 1;

//     float accumulator = 0.0f;
//     const float FIXED_TIMESTEP = 1.0f / 60.0f;

//     std::mutex sceneMutex;
// };#pragma once

// #include <btBulletDynamicsCommon.h>
// #include <BulletDynamics/Character/btKinematicCharacterController.h>

// struct ServerInput
// {
//     int direction;
//     bool jumping;
//     double timestamp;
// };

// class ServerPlayer
// {
// public:
//     ServerPlayer(uint32_t id, btPairCachingGhostObject *object, btKinematicCharacterController *controller);
//     ~ServerPlayer();

//     uint32_t GetID() const;
//     btKinematicCharacterController *GetController() const;

//     void ApplyInput(const ServerInput &input);

//     void PostPhysicsUpdate(float deltaTime);

//     btVector3 GetPosition() const;

// private:
//     uint32_t id;

//     btPairCachingGhostObject* ghost;
//     btKinematicCharacterController* controller;

//     btVector3 pendingWalkDir;
//     bool pendingJump = false;

//     ServerInput lastInput;
// };