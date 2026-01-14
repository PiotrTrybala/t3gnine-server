// #include "player.hpp"

// Player::Player(uint32_t id) : id(id)
// {
//     btConvexShape *shape = new btCapsuleShape(DEFAULT_CAPSULE_PLAYER_SHAPE.x, DEFAULT_CAPSULE_PLAYER_SHAPE.y);
//     ghostObject = new btPairCachingGhostObject();

//     btTransform trans;
//     trans.setOrigin(btVector3(0, 0, 0));
//     // TODO: Set rotation later

//     ghostObject->setWorldTransform(trans);
//     ghostObject->setCollisionShape(shape);
//     ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
//     ghostObject->setUserPointer(this);

//     controller = new btKinematicCharacterController(ghostObject, shape, 0.25f);

//     lastInput = {
//         .id = 0,
//         .sequence = 0,
//         .front = glm::vec3(0.0f, 0.0f, 0.0f),
//         .directionMask = 0,
//         .isJumping = false,
//     };
// }
// Player::~Player()
// {
//     delete ghostObject;
//     delete controller;
// }

// const uint32_t Player::GetID()
// {
//     return this->id;
// }
// const btVector3 &Player::GetPosition()
// {
//     return ghostObject->getWorldTransform().getOrigin();
// }

// void Player::ApplyInput(const PlayerInput &input)
// {
//     lastInput = input;

//     glm::vec3 up(0.0f, 1.0f, 0.0f);
//     glm::vec3 right = glm::normalize(glm::cross(up, input.front));

//     glm::vec3 movementDirection(0.0f);

//     if (input.directionMask & WalkDirection::FRONT)
//         movementDirection += input.front;
//     if (input.directionMask & WalkDirection::BACK)
//         movementDirection -= input.front;
//     if (input.directionMask & WalkDirection::RIGHT)
//         movementDirection += right;
//     if (input.directionMask & WalkDirection::LEFT)
//         movementDirection -= right;

//     if (glm::length(movementDirection) > 0.001f)
//     {
//         movementDirection = glm::normalize(movementDirection);
//     }

//     this->Update(GlmToBt(movementDirection), input.isJumping);
// }
// void Player::Update(const btVector3 walkDirection, bool isJumping)
// {

//     float speed = 4.0f;
//     controller->setWalkDirection(walkDirection * speed * FIXED_DELTA_TIME);

//     if (isJumping)
//     {
//         if (controller->canJump() && controller->onGround())
//         {
//             controller->jump(btVector3(0, controller->getJumpSpeed(), 0));
//         }
//     }
// }