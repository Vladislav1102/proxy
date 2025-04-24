#include "proxy.hpp"
#include <netinet/in.h>

volatile sig_atomic_t gSignalStatus;

Proxy::Proxy() {}

Proxy::~Proxy() 
{
    close(socket_client);
    close(connect_client);
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

void Proxy::install_settings_sockets(struct sockaddr_in* device, in_addr_t& address, uint16_t port)
{
    device->sin_addr.s_addr = address;
    device->sin_family      = AF_INET;
    device->sin_port        = htons(port);
}

void Proxy::setup_client() 
{
    this->socket_client = open_socket();

    install_settings_sockets(&client_addr, address_client, port);
    memset(&(client_addr.sin_zero), '\0', 8);

    if (bind(socket_client, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Error binding socket for client" << std::endl;
    }

    listen(socket_client, SOMAXCONN);
}

void Proxy::setup_target()
{
    this->socket_target = open_socket();

    std::string input_addres;
    std::cout << "Введите адрес целевого сервера" << std::endl;
    std::cin >> input_addres;

    if (inet_pton(AF_INET, input_addres.c_str(),&address_target) <= 0) 
    {
        std::cerr << "Error inet_pton" << std::endl;
    }
    install_settings_sockets(&target_addr, address_target, port_target);
}

void Proxy::connect_client_to_server()
{
    connect_client = accept(socket_client, nullptr, nullptr);
    if (connect_client <= 0)
    {
        std::cerr << "Error accept client to proxy" << std::endl;
    }

    if (connect(socket_target, (struct sockaddr*)&target_addr, sizeof(target_addr)) < 0)
    {
        std::cerr << "Error connecting proxy to target" << std::endl;
        close(socket_client);
    } else {
        std::cout << "Connect client for target server succsses" << std::endl;
    }

    std::cout << "Client accept to target" << std::endl;
}

void Proxy::transfer_data()
{
    char data_client[MAX_SIZE] = {};
    char data_target[MAX_SIZE] = {};
    struct pollfd fds[2];
    fds[0].fd = connect_client;
    fds[0].events = POLLIN;

    fds[1].fd = socket_target;
    fds[1].events = POLLIN;

    while (true) 
    {
        int ret = poll(fds, 2, -1);

        if (ret < 0)
        {
            std::cerr << "Error poll" << std::endl;
        }

        if (fds[0].revents & POLLIN)
        {
            ssize_t len = read(connect_client, data_client, sizeof(data_client));
            if (len <= 0) break;
            send(socket_target, data_client, len, 0);
            std::cout << "[CLIENT: ]" << std::string(data_client, len);
        }

        if (fds[1].revents & POLLIN)
        {
            ssize_t len = read(socket_target, data_target, sizeof(data_target));
            if (len <= 0) break;
            send(connect_client, data_target, len, 0);
            std::cout << "[SERVER: ]"<< std::string(data_target, len);
        }
    } 
}

void Proxy::run_proxy()
{
    setup_client();

    setup_target();
    connect_client_to_server();

    while(true)
    {
        transfer_data();
    }
}