#pragma once

#include <unordered_map>
#include <cstdint>

#include "player.hpp"

class Scene
{
public:
    Scene();
    ~Scene();

private:
    std::unordered_map<uint32_t, Player *> players;

    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btDbvtBroadphase *broadphase;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};