#include "server.hpp"

GameServer::GameServer(uint16_t port) : workGuard(asio::make_work_guard(ioContext)), socket(ioContext, udp::endpoint(udp::v4(), port)), tickTimer(ioContext)
{
    scene = std::make_unique<PhysicsScene>();

    std::cout << "[SERVER] Started on port " << port << std::endl;

    StartReceive();

    serverThread = std::thread([this]{ ioContext.run(); });

    running = true;
}

GameServer::~GameServer()
{
    running = false;

    workGuard.reset();
    ioContext.stop();

    if (serverThread.joinable())
    {
        serverThread.join();
    }
}

void GameServer::Run()
{
    Tick();
}

void GameServer::Broadcast(const Packet &packet)
{
    for(auto& [id, endpoint] : endpoints)
        Send(packet, endpoint);
}

void GameServer::Send(const Packet &packet, const udp::endpoint &endpoint)
{
    socket.async_send_to(asio::buffer(packet.data), endpoint, [](std::error_code, std::size_t){});
}

void GameServer::Tick() {
    
    if (!running) return;

    scene->Update(TICK_RATE);

    for (auto& [id, player] : players) {
        Packet packet;
        packet.Write(PacketType::Reconcile);
        packet.Write(id);
        packet.Write(player->GetPosition().x());
        packet.Write(player->GetPosition().y());
        packet.Write(player->GetPosition().z());
        packet.Write(player->GetLastSequence());

        Broadcast(packet);
    }
}

void GameServer::StartReceive()
{
    socket.async_receive_from(asio::buffer(receiveBuffer), remoteEndpoint, [this](std::error_code& ec, std::size_t bytesReceived){
        if (!ec && bytesReceived > 0) {
            Packet packet;
            packet.data.assign(receiveBuffer, receiveBuffer + bytesReceived);

            HandleReceive(packet, remoteEndpoint);
        }
        StartReceive();
    });
}

void GameServer::HandleReceive(Packet &packet, const udp::endpoint &endpoint)
{
    PacketType type;
    if (!packet.Read(type)) 
        return;

    switch(type) {
        case PacketType::PlayerJoin:
        {

            uint32_t id = nextPlayerId++;
            endpoints[id] = endpoint;

            players[id] = std::make_unique<Player>(id);

            std::cout << "[SERVER] Player joined ID = " << id << std::endl;
            {
                Packet response;
                response.Write(PacketType::PlayerJoinACK);
                response.Write(id);
                Send(response, endpoint);
            }

            for (auto& [eId, player] : players) {

                if (eId == id) continue;

                Packet response;
                response.Write(PacketType::PlayerJoined);
                response.Write(id);
                response.Write(player->GetPosition());
                Send(response, endpoint);

            }

            {
                Packet response;
                response.Write(PacketType::PlayerJoined);
                response.Write(id);
                response.Write(players[id]->GetPosition());
                Broadcast(response);
            }

            break;
        }
        case PacketType::PlayerLeave:
        {
            // TODO: Handle case when player leaves server
            break;
        }
        case PacketType::PlayerInput:
        {

            uint32_t id;
            uint32_t sequence;
            glm::vec3 direction;
            uint8_t directionMask;
            bool isJumping;

            if (!packet.Read(id)) return;
            if (!packet.Read(sequence)) return;
            if (!packet.Read(direction.x)) return;
            if (!packet.Read(direction.y)) return;
            if (!packet.Read(direction.z)) return;
            if (!packet.Read(directionMask)) return;
            if (!packet.Read(isJumping)) return;

            auto it = players.find(id);
            if (it == players.end()) return;

            Player* player = it->second.get();

            PlayerInput input = { id, sequence, direction, directionMask, isJumping };

            player->ApplyInput(input);
            break;
        }
    }
}