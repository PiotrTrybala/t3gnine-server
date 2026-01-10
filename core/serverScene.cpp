// #include "gameScene.hpp"

// GameScene::GameScene()
// {
//     collisionConfig = new btDefaultCollisionConfiguration();
//     dispatcher = new btCollisionDispatcher(collisionConfig);
//     broadphase = new btDbvtBroadphase();
//     solver = new btSequentialImpulseConstraintSolver();
//     world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

//     world->setGravity(btVector3(0, -9.81f, 0));
// }
// GameScene::~GameScene()
// {
//     for (int i = 0; i < world->getCollisionObjectArray().size(); i++)
//     {
//         world->removeCollisionObject(world->getCollisionObjectArray()[i]);
//     }
//     colliders.clear();
//     rigidBodies.clear();
//     players.clear();

//     delete world;
//     delete solver;
//     delete broadphase;
//     delete dispatcher;
//     delete collisionConfig;
// }

// void GameScene::Step(float deltaTime)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     accumulator += deltaTime;

//     while (accumulator >= FIXED_TIMESTEP)
//     {

//         world->stepSimulation(FIXED_TIMESTEP, 1);

//         for (auto &[id, player] : players)
//         {
//             player->PostPhysicsUpdate(FIXED_TIMESTEP);
//         }

//         accumulator -= FIXED_TIMESTEP;
//     }
// }

// uint32_t GameScene::AddRigidBody(btRigidBody *body)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     uint32_t id = nextID++;
//     rigidBodies[id] = body;

//     world->addRigidBody(body);
//     return id;
// }
// void GameScene::RemoteRigidBody(uint32_t id)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     auto it = rigidBodies.find(id);
//     if (it == rigidBodies.end())
//         return;

//     world->removeRigidBody(it->second);
//     rigidBodies.erase(it);
// }

// uint32_t GameScene::AddCollider(btCollisionObject *object)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     uint32_t id = nextID++;
//     colliders[id] = object;

//     world->addCollisionObject(object);
//     return id;
// }
// void GameScene::RemoveCollider(uint32_t id)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     auto it = colliders.find(id);
//     if (it == colliders.end())
//         return;

//     world->removeCollisionObject(it->second);
//     colliders.erase(it);
// }

// void GameScene::AddPlayer(std::shared_ptr<ServerPlayer> player)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);
//     players[player->GetID()] = player;

//     if (player->GetController())
//         world->addCharacter(player->GetController());
// }
// void GameScene::RemovePlayer(uint32_t id)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     auto it = players.find(id);
//     if (it == players.end())
//         return;

//     if (it->second->GetController())
//         world->removeCharacter(it->second->GetController());

//     players.erase(id);
// }

// std::shared_ptr<ServerPlayer> GameScene::GetPlayer(uint32_t id)
// {
//     std::lock_guard<std::mutex> lock(sceneMutex);

//     auto it = players.find(id);
//     return (it != players.end()) ? it->second : nullptr;
// }