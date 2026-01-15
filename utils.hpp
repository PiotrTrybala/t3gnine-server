#pragma once

#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>

typedef btCollisionObject Collider;
typedef btRigidBody RigidBody;

inline static btVector3 GlmToBt(const glm::vec3 &vec)
{
    return btVector3(vec.x, vec.y, vec.z);
}
inline static glm::vec3 BtToGlm(const btVector3 &vec)
{
    return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

inline static Collider *CreateCollider(const btVector3 &position, const btVector3 &scale)
{
    auto *shape = new btBoxShape(scale);

    auto *collider = new btCollisionObject();
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(position);

    collider->setWorldTransform(transform);
    collider->setCollisionShape(shape);

    return collider;
}

inline static RigidBody *CreateRigidBody(const btVector3 &position, const btVector3 &scale, float mass)
{
btCollisionShape* shape = new btBoxShape(scale);

    btVector3 inertia(0,0,0);
    if (mass > 0.0f)
        shape->calculateLocalInertia(mass, inertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(position);

    btDefaultMotionState* motionState = new btDefaultMotionState(transform);

    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, inertia);
    btRigidBody* body = new btRigidBody(info);

    return body;
}
