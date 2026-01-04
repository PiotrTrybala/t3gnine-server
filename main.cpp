#include <iostream>

#include "core/gameServer.hpp"

int main()
{
    GameServer server(10090);
    server.Run();

    std::cout << "Server running. Press Enter to quit...\n";
    std::cin.get();
    return 0;
}