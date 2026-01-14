// #pragma once

// #include <unordered_map>
// #include <cstdint>

// #include "player.hpp"

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