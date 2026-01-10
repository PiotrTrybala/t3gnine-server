#pragma once

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

class ServerPlayer {
    public:
        ServerPlayer(uint32_t id);
        ~ServerPlayer();

        const uint32_t GetID();
        btKinematicCharacterController* GetController();

        void ApplyInput(const PlayerInput& input);
        void UpdatePhysics();

        btVector3 GetPosition();
    private:
        uint32_t id;

        btPairCachingGhostObject* ghost;
        btKinematicCharacterController* controller;

        btVector3 walkDirection;
        bool jump = false;

        PlayerInput lastInput;
};