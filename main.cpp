#include <iostream>

#include "core/gameServer.hpp"

int main()
{
    GameServer server(10090);
    std::cin.get();
    return 0;
}