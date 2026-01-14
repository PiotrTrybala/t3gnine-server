// #include "scene.hpp"

// Scene::Scene()
// {
//     collisionConfiguration = new btDefaultCollisionConfiguration();
//     dispatcher = new btCollisionDispatcher(collisionConfiguration);
//     broadphase = new btDbvtBroadphase();
//     solver = new btSequentialImpulseConstraintSolver();
//     dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

//     dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
// }
// Scene::~Scene()
// {
//     for (auto &[id, body] : rigidBodies)
//         dynamicsWorld->removeRigidBody(body);

//     rigidBodies.clear();
//     colliders.clear();
//     players.clear();

//     delete dynamicsWorld;
//     delete solver;
//     delete broadphase;
//     delete dispatcher;
//     delete collisionConfiguration;
// }

// void Scene::AddCollider(const btVector3& position, const btVector3& scale)
// {
// }
// void Scene::RemoveCollider(uint32_t id)
// {
// }

// void Scene::AddRigidBody(btRigidBody *body)
// {
// }
// void Scene::RemoveRigidBody(uint32_t id)
// {
// }

// void Scene::AddPlayer(Player *player)
// {
//     players[player->GetID()] = player;
// }
// void Scene::RemovePlayer(uint32_t id)
// {
//     auto it = players.find(id);
//     if (it == players.end()) return;

//     players.erase(id);
// }

// void Scene::Update(float deltaTime)
// {




    
// }