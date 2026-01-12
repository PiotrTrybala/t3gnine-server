#pragma once

#include <cstdint>

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "types.hpp"
#include "tag.hpp"

static btVector3 GlmToBt(const glm::vec3 &vec)
{
    return btVector3(vec.x, vec.y, vec.z);
}
static glm::vec3 BtToGlm(const btVector3 &vec)
{
    return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

#pragma once

class Player
{
public:
    Player(uint32_t id);
    ~Player();

    const uint32_t GetID();
    const btVector3& GetPosition();
    void SetPosition(const btVector3& position);

    void ApplyInput(const PlayerInput& input);
    void Update(const btVector3 walkDirection, bool isJumping);

private:
    uint32_t id;

    btPairCachingGhostObject* ghostObject;
    btKinematicCharacterController* controller;

    PlayerInput lastInput;
};