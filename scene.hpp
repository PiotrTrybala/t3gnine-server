#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <unordered_map>
#include <cstdint>

#include "player.hpp"
#include "utils.hpp"

class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    Collider *GetCollider(uint32_t id);
    uint32_t AddCollider(Collider* collider);
    void RemoveCollider(uint32_t id);

    Player *GetPlayer(uint32_t id);
    void AddPlayer(Player *player);
    void RemovePlayer(uint32_t id);

    RigidBody* GetRigidBody(uint32_t id);
    void AddRigidBody(RigidBody* rigidBody);
    void RemoveRigidBody(uint32_t id);

    void Update(float deltaTime);

private:
    std::unordered_map<uint32_t, Collider *> colliders;
    std::unordered_map<uint32_t, RigidBody *> rigidBodies;
    std::unordered_map<uint32_t, Player *> players;

    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btDbvtBroadphase *broadphase;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};

// class Scene
// {
// public:
//     Scene();
//     ~Scene();

//     void AddCollider(const btVector3& position, const btVector3& scale);
//     void RemoveCollider(uint32_t id);

//     void AddRigidBody(btRigidBody* body);
//     void RemoveRigidBody(uint32_t id);

//     void AddPlayer(Player* player);
//     void RemovePlayer(uint32_t id);

//     void Update(float deltaTime);

// private:
//     std::unordered_map<uint32_t, btCollisionObject *> colliders;
//     std::unordered_map<uint32_t, btRigidBody *> rigidBodies;
//     std::unordered_map<uint32_t, Player *> players;

//     btDefaultCollisionConfiguration *collisionConfiguration;
//     btCollisionDispatcher *dispatcher;
//     btDbvtBroadphase *broadphase;
//     btSequentialImpulseConstraintSolver *solver;
//     btDiscreteDynamicsWorld *dynamicsWorld;
// };