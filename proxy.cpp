#include "proxy.hpp"
#include <netinet/in.h>

volatile sig_atomic_t gSignalStatus;

Proxy::Proxy() {}

Proxy::~Proxy() {}

uint16_t Proxy::open_socket()
{
    uint16_t socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd < 0)
    {
        throw "Error opening socket";
        return EXIT_FAILURE;
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
    {
        throw "Error set settings socket";
        return EXIT_FAILURE;
    }
    
    return socketfd;
}

void Proxy::install_settings_sockets(struct sockaddr_in* device, in_addr_t* address)
{
    device->sin_addr.s_addr = *address;
    device->sin_family      = AF_INET;
    device->sin_port        = htons(port);
}

void Proxy::setup_host() 
{
    socket_host = open_socket();
    install_settings_sockets(&host, &address_host);

    memset(&(host.sin_zero), '\0', 8);

    if (bind(socket_host, (struct sockaddr *)&host, sizeof(struct sockaddr)) < 0)
    {
        throw "Error binding socket";
    }

    listen(socket_host, SOMAXCONN);
}

void Proxy::setup_target(std::string& input_addres)
{
    socket_target = open_socket();
    inet_pton(AF_INET, input_addres.c_str(), &address_target);
    install_settings_sockets(&target, &address_target);
}

void Proxy::handler()
{
    char data[MAX_SIZE] = {};
    size_t size_data;

    while ((size_data = read(socket_host, data, sizeof(data))) > 0)
    {
        send(socket_target, data, size_data, 0);
    }
}

void Proxy::run_proxy()
{
    std::string input_addres;
    std::cout << "Введите адрес целевого сервера" << std::endl;
    std::cin >> input_addres;
}