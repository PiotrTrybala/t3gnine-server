#include "player.hpp"

Player::Player(uint32_t id) : id(id)
{
    btCollisionShape* shape = new btCapsuleShape(DEFAULT_CAPSULE_PLAYER_SHAPE.x, DEFAULT_CAPSULE_PLAYER_SHAPE.y);
    ghost = new btPairCachingGhostObject();

    btTransform trans;
    trans.setOrigin(btVector3(0, 0, 0));
    // TODO: Set rotation later

    ghost->setWorldTransform(trans);
    ghost->setCollisionShape(shape);
    ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    ghost->setUserPointer(this);

    controller = new btKinematicCharacterController(ghost, shape, 0.25f);

    lastInput = {
        .direction = 0,
        .front = glm::vec3(0.0f),
        .isJumping = false,
        .timestamp = 0.0f,
    };
}
Player::~Player()
{
    delete controller;
    delete ghost;
}

const uint32_t Player::GetID()
{
    return this->id;
}
const btVector3 &Player::GetPosition()
{
    btTransform trans = ghost->getWorldTransform();
    return trans.getOrigin();
}

void Player::ApplyInput(const PlayerInput &input)
{
    lastInput = input;

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(up, input.front));

    glm::vec3 movementDirection;

    if (input.direction & WalkDirection::FRONT)
        movementDirection += input.front;
    if (input.direction & WalkDirection::BACK)
        movementDirection -= input.front;
    if (input.direction & WalkDirection::RIGHT)
        movementDirection += right;
    if (input.direction & WalkDirection::LEFT)
        movementDirection -= right;
    
    float speed = 4.0f;

    movementDirection = glm::normalize(movementDirection);

    this->Update(GlmToBt(movementDirection), input.isJumping);

}
void Player::Update(const btVector3 walkDirection, bool isJumping)
{

    controller->setWalkDirection(btVector3(walkDirection.x(), 0, walkDirection.z()) * controller->getMaxSlope());

    if (isJumping) {
        if (controller->canJump() && controller->onGround()) {
            controller->jump(btVector3(0, controller->getJumpSpeed(), 0));
        }
    }


}