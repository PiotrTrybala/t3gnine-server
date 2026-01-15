#include "player.hpp"

Player::Player(uint32_t id, glm::vec3 position) : id(id), position(position)
{

    shape = new btCapsuleShape(DEFAULT_CAPSULE_PLAYER_SHAPE.x, DEFAULT_CAPSULE_PLAYER_SHAPE.y);

    ghostObject = new btPairCachingGhostObject();
    ghostObject->setCollisionShape(shape);
    ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(GlmToBt(position));
    ghostObject->setWorldTransform(trans);

    // world->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);

    controller = new btKinematicCharacterController(ghostObject, shape, 0.25f);

    // world->addAction(controller);
}
Player::~Player()
{
    world->removeAction(controller);
    world->removeCollisionObject(ghostObject);

    delete controller;
    delete ghostObject;
    delete shape;
}

const uint32_t Player::GetID()
{
    return this->id;
}
const glm::vec3 &Player::GetPosition()
{
    return this->position;
}

void Player::PushInput(const PlayerInput &input)
{
    pendingInputs.push(input);
}

void Player::ApplyInput(float deltaTime)
{
    while (!pendingInputs.empty())
    {
        const PlayerInput &input = pendingInputs.front();

        glm::vec3 walkDirection(0.0);
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


        controller->setWalkDirection(GlmToBt(walkDirection * moveSpeed * deltaTime));
        
        if (input.isJumping && controller->canJump() && controller->onGround()) {
            controller->jump();
        }

        lastSequence = input.sequence;
        pendingInputs.pop();
    }
}
void Player::Update()
{
    btTransform trans = ghostObject->getWorldTransform();
    btVector3 position = trans.getOrigin();
    this->position = BtToGlm(position);
}