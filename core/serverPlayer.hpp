// #pragma once

// #include <btBulletDynamicsCommon.h>
// #include <BulletDynamics/Character/btKinematicCharacterController.h>

// struct ServerInput
// {
//     int direction;
//     bool jumping;
//     double timestamp;
// };

// class ServerPlayer
// {
// public:
//     ServerPlayer(uint32_t id, btPairCachingGhostObject *object, btKinematicCharacterController *controller);
//     ~ServerPlayer();

//     uint32_t GetID() const;
//     btKinematicCharacterController *GetController() const;

//     void ApplyInput(const ServerInput &input);

//     void PostPhysicsUpdate(float deltaTime);

//     btVector3 GetPosition() const;

// private:
//     uint32_t id;

//     btPairCachingGhostObject* ghost;
//     btKinematicCharacterController* controller;

//     btVector3 pendingWalkDir;
//     bool pendingJump = false;

//     ServerInput lastInput;
// };