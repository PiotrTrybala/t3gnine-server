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

inline static Collider* CreateCollider(const btVector3& position, const btVector3& scale) {

}

inline static RigidBody* CreateRigidBody(const btVector3& position, const btVector3& scale, float mass) {

}

