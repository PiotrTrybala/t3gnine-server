#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <glm/glm.hpp>

#include "tag.hpp"

btKinematicCharacterController *CreateDefaultController(const btVector3 initialPosition, const btQuaternion initialRotation)
{
    btCapsuleShape *shape = new btCapsuleShape(DEFAULT_CAPSULE_PLAYER_SHAPE.x, DEFAULT_CAPSULE_PLAYER_SHAPE.y);
    btPairCachingGhostObject *ghost = new btPairCachingGhostObject();

    btTransform trans;
    trans.setOrigin(initialPosition);
    trans.setRotation(initialRotation);

    ghost->setWorldTransform(trans);
    ghost->setCollisionShape(shape);
    ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    
    return new btKinematicCharacterController(ghost, shape, 0.25f);
}