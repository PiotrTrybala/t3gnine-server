#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

#include <btBulletDynamicsCommon.h>

#include "serverPlayer.hpp"

class ServerScene
{
public:
    ServerScene();
    ~ServerScene();

    void Update(float deltaTime);

    btRigidBody *GetRigidBody(uint32_t id);
    uint32_t AddRigidBody(btRigidBody *body);
    void RemoteRigidBody(uint32_t id);

    btCollisionObject *GetCollider(uint32_t id);
    uint32_t AddCollider(btCollisionObject *object);
    void RemoveCollider(uint32_t id);

    std::shared_ptr<ServerPlayer> GetPlayer(uint32_t id);
    void AddPlayer(std::shared_ptr<ServerPlayer> player);
    void RemovePlayer(uint32_t id);

private:
    btDefaultCollisionConfiguration *collisionConfig;
    btCollisionDispatcher *dispatcher;
    btBroadphaseInterface *broadphase;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;

    std::unordered_map<uint32_t, btCollisionObject *> colliders;
    std::unordered_map<uint32_t, btRigidBody *> rigidBodies;
    std::unordered_map<uint32_t, std::shared_ptr<ServerPlayer>> players;

    uint32_t nextId = 1;

    float accumulator = 0.0f;
    const float FIXED_TIMESTAMP = 1.0f / 60.0f;

    std::mutex sceneMutex;
};
