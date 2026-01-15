#include "scene.hpp"

PhysicsScene::PhysicsScene()
{
}
PhysicsScene::~PhysicsScene()
{
}

Collider *PhysicsScene::GetCollider(uint32_t id)
{
}
uint32_t PhysicsScene::AddCollider(Collider *collider)
{
}
void PhysicsScene::RemoveCollider(uint32_t id)
{
}

Player *PhysicsScene::GetPlayer(uint32_t id)
{
}
void PhysicsScene::AddPlayer(Player *player)
{
}
void PhysicsScene::RemovePlayer(uint32_t id)
{
}

RigidBody *PhysicsScene::GetRigidBody(uint32_t id)
{
}
void PhysicsScene::AddRigidBody(RigidBody *rigidBody)
{
}
void PhysicsScene::RemoveRigidBody(uint32_t id)
{
}

void PhysicsScene::Update(float deltaTime)
{
}