#include "proxy.hpp"

volatile sig_atomic_t gSignalStatus;

Proxy::Proxy() {}

Proxy::~Proxy() {}

uint16_t Proxy::open_socket()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

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

uint16_t Proxy::setup_host() 
{
    uint16_t sock_host = open_socket();

    host.sin_addr.s_addr = INADDR_ANY;
    host.sin_family      = AF_INET;
    host.sin_port        = htons(port);

    memset(&(host.sin_zero), '\0', 8);

    if (bind(socketfd, (struct sockaddr *)&host, sizeof(struct sockaddr)) < 0)
    {
        throw "Error binding socket";
    }

    listen(sock_host, SOMAXCONN);
    return sock_host;
}

uint16_t Proxy::setup_target()
{
    std::string input_addres;
    uint16_t input_port = 0;

    std::cout << "Введите адрес целевого сервера" << std::endl;
    std::cin >> input_addres;

    std::cout << "Введите порт" << std::endl;
    std::cin >> input_port;

    uint16_t sock_target = open_socket();

    inet_pton(AF_INET, input_addres.c_str(), &target.sin_addr.s_addr);
    target.sin_family = AF_INET;
    target.sin_port   = htons(input_port);

    return sock_target;
}

void Proxy::handler(uint16_t socket_host, uint16_t socket_target)
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
    while (1) 
    {
        uint16_t host_socket   = setup_host();
        uint16_t target_socket = setup_target();

        if (connect(target_socket, (struct sockaddr*) &target, sizeof(sockaddr)) < 0)
        {
            std::cout << "Error connecting client" << std::endl;
        }
        std::cout << "Client connected" << std::endl;
        handler(host_socket,  target_socket);
    }
}