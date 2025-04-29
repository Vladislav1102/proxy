#include "proxy.hpp"

RealServer::RealServer() 
{
    serverfd = open_socket();
}

RealServer::~RealServer()
{
    close(serverfd);
}

void RealServer::setup()
{
    std::cout << "Введите адрес сервера: ";
    std::cin >> input_address_server;

    if (inet_pton(AF_INET, input_address_server.c_str(), &address_server) < 0)
    {
        std::cerr << "Error inet_pton Server" << std::endl;
    }

    settings_socket(&server_addr, address_server, port_server);
}
