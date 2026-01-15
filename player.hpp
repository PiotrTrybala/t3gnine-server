#pragma once

#include <iostream>
#include <cstdint>
#include <queue>

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "types.hpp"
#include "utils.hpp"
#include "tag.hpp"

class Player
{
    friend class PhysicsScene;

public:
    Player(uint32_t id, glm::vec3 position);
    ~Player();

    const uint32_t GetID();
    const glm::vec3& GetPosition();

    void PushInput(const PlayerInput& input);

    void ApplyInput(float deltaTime);
    void Update();

private:

    uint32_t id;
    uint32_t lastSequence;

    btPairCachingGhostObject* ghostObject;
    btConvexShape* shape;
    btKinematicCharacterController* controller;

    btDiscreteDynamicsWorld *world;

    float moveSpeed = 6.0f;

    glm::vec3 position;

    std::queue<PlayerInput> pendingInputs;
};

// class Player
// {
// public:
//     Player(uint32_t id);
//     ~Player();

//     const uint32_t GetID();
//     const uint32_t GetLastSequence();
//     const btVector3& GetPosition();
//     void SetPosition(const btVector3& position);

//     void ApplyInput(const PlayerInput& input);
//     void Update(const btVector3 walkDirection, bool isJumping);

// private:
//     uint32_t id;
//     uint32_t lastSequence;

//     btPairCachingGhostObject* ghostObject;
//     btKinematicCharacterController* controller;
// };