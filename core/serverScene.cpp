#include "serverScene.hpp"

ServerScene::ServerScene()
{
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

    world->setGravity(btVector3(0, -9.81f, 0));
}
ServerScene::~ServerScene()
{
    for (int i = 0; i < world->getCollisionObjectArray().size(); i++)
    {
        world->removeCollisionObject(world->getCollisionObjectArray()[i]);
    }

    colliders.clear();
    rigidBodies.clear();
    players.clear();

    delete world;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collisionConfig;
}

void ServerScene::Update(float deltaTime)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    accumulator += deltaTime;

    while (accumulator >= FIXED_TIMESTAMP)
    {
        world->stepSimulation(FIXED_TIMESTAMP, 1);

        for (auto &[id, player] : players)
        {
            player->UpdatePhysics();
        }
        accumulator -= FIXED_TIMESTAMP;
    }
}

btRigidBody *ServerScene::GetRigidBody(uint32_t id)
{
    std::lock_guard<std::mutex> lock(sceneMutex);
    auto it = rigidBodies.find(id);
    if (it == rigidBodies.end())
        return nullptr;
    return it->second;
}
uint32_t ServerScene::AddRigidBody(btRigidBody *body)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    uint32_t id = nextId++;
    rigidBodies[id] = body;

    world->addRigidBody(body);
    return id;
}
void ServerScene::RemoteRigidBody(uint32_t id)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    auto it = rigidBodies.find(id);
    if (it == rigidBodies.end())
        return;

    world->removeRigidBody(it->second);
    rigidBodies.erase(it);
}

btCollisionObject *ServerScene::GetCollider(uint32_t id)
{
    std::lock_guard<std::mutex> lock(sceneMutex);
    auto it = colliders.find(id);
    if (it == colliders.end())
        return nullptr;
    return it->second;
}
uint32_t ServerScene::AddCollider(btCollisionObject *object)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    uint32_t id = nextId++;
    colliders[id] = object;

    world->addCollisionObject(object);
    return id;
}
void ServerScene::RemoveCollider(uint32_t id)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    auto it = colliders.find(id);
    if (it == colliders.end())
        return;

    world->removeCollisionObject(it->second);
    colliders.erase(it);
}

std::shared_ptr<ServerPlayer> ServerScene::GetPlayer(uint32_t id)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    auto it = players.find(id);
    return (it != players.end()) ? it->second : nullptr;
}
void ServerScene::AddPlayer(std::shared_ptr<ServerPlayer> player)
{
    std::lock_guard<std::mutex> lock(sceneMutex);
    players[player->GetID()] = player;

    if (player->GetController())
        world->addCharacter(player->GetController());
}
void ServerScene::RemovePlayer(uint32_t id)
{
    std::lock_guard<std::mutex> lock(sceneMutex);

    auto it = players.find(id);
    if (it == players.end())
        return;

    if (it->second->GetController())
        world->removeCharacter(it->second->GetController());

    players.erase(id);
}