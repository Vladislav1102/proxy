#include "proxy.hpp"
#include <netinet/in.h>

volatile sig_atomic_t gSignalStatus;

Proxy::Proxy() {}

Proxy::~Proxy() 
{
    close(socket_client);
    close(socket_target);
}

int32_t Proxy::open_socket()
{
    uint16_t socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd < 0)
    {
        std::cerr << "Error opening socket";
        return EXIT_FAILURE;
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
    {
        std::cerr << "Error set settings socket" << std::endl;
        return EXIT_FAILURE;
    }
    
    return socketfd;
}

void Proxy::install_settings_sockets(struct sockaddr_in* device, in_addr_t* address)
{
    device->sin_addr.s_addr = htonl(*address);
    device->sin_family      = AF_INET;
    device->sin_port        = htons(port);
}

void Proxy::setup_host() 
{
    this->socket_client = open_socket();

    install_settings_sockets(&client_addr, &address_client);
    memset(&(client_addr.sin_zero), '\0', 8);

    if (bind(socket_client, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
    }

    listen(socket_client, SOMAXCONN);
}

void Proxy::setup_target(const std::string& input_addres)
{
    this->socket_target = open_socket();

    inet_pton(AF_INET, input_addres.c_str(), &address_target);
    install_settings_sockets(&target_addr, &address_target);
}

void Proxy::connect_client_to_server()
{
    this->socket_client = accept(socket_target, nullptr, nullptr);

    if (connect(socket_target, (struct sockaddr*)&target_addr, sizeof(target_addr)) < 0)
    {
        std::cerr << "Error connecting client to server" << std::endl;
        close(socket_client);
    }
}

void Proxy::poller()
{
    char data[MAX_SIZE] = {};
    size_t size_data;

    while ((size_data = read(socket_client, data, sizeof(data))) > 0)
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