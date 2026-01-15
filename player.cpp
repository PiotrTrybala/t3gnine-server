#include "player.hpp"

Player::Player(uint32_t id) : id(id)
{
    btConvexShape *shape = new btCapsuleShape(DEFAULT_CAPSULE_PLAYER_SHAPE.x, DEFAULT_CAPSULE_PLAYER_SHAPE.y);
    ghostObject = new btPairCachingGhostObject();

    btTransform trans;
    trans.setOrigin(btVector3(0, 0, 0));

    ghostObject->setWorldTransform(trans);
    ghostObject->setCollisionShape(shape);
    ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    ghostObject->setUserPointer(this);

    controller = new btKinematicCharacterController(ghostObject, shape, 0.25f);

    lastInput = {0, 0, glm::vec3(0.0f), 0, false};
    lastSequence = 0;
}
Player::~Player()
{
    delete controller;
}

const uint32_t Player::GetID()
{
    return id;
}
const uint32_t Player::GetLastSequence()
{
    return lastSequence;
}
const btVector3 &Player::GetPosition()
{
    return ghostObject->getWorldTransform().getOrigin();
}
void Player::SetPosition(const btVector3 &position)
{
    btTransform trans;
    trans.setOrigin(position);

    ghostObject->setWorldTransform(trans);
}

void Player::ApplyInput(const PlayerInput &input)
{
    lastInput = input;
    lastSequence = input.sequence;

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(up, input.direction));

    glm::vec3 walkDirection(0.0f);

    if (input.directionMask & WalkDirection::FRONT)
        walkDirection += input.direction;
    if (input.directionMask & WalkDirection::BACK)
        walkDirection -= input.direction;
    if (input.directionMask & WalkDirection::RIGHT)
        walkDirection += right;
    if (input.directionMask & WalkDirection::LEFT)
        walkDirection -= right;

    if (glm::length(walkDirection) > 0.001f) {
        walkDirection = glm::normalize(walkDirection);
    }

    Update(GlmToBt(walkDirection), input.isJumping);
}
void Player::Update(const btVector3 walkDirection, bool isJumping)
{

    const float speed = 4.0f; // TODO: Place this value somewhere global

    controller->setWalkDirection(walkDirection * speed * FIXED_DELTA_TIME);

    if (isJumping) {

        if (!controller->canJump() || !controller->onGround()) return;

        controller->jump(btVector3(0, 4, 0)); // TODO: Place this vector somewhere global

    }

}