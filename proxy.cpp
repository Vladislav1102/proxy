#include "proxy.hpp"

uint16_t open_socket()
{
    int optval = 1;
    int new_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (new_socket < 0)
    {
        std::cerr << "Error open socket" << std::endl;
    } 
    else
    {
        std::cout << "New socket opened" << std::endl;
    } 

    if (setsockopt(new_socket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
    {
        perror("Error set socket");
        close(new_socket);
    }
    else
    {
        std::cout << "Socket setsock" << std::endl;
    }
    return new_socket;
}

void settings_socket(struct sockaddr_in* device, in_addr_t& address, int16_t port)
{
    memset(device, 0, sizeof(sockaddr_in));
    device->sin_family      = AF_INET;
    device->sin_addr.s_addr = address;
    device->sin_port        = htons(port);
}

Proxy::Proxy()
{
    fdRx = open_socket();
    fdTx = open_socket();
}

Proxy::~Proxy()
{
    close(fdTx);
    close(fdRx);
}

void Proxy::set_nonblockfd(int16_t fd)
{
    int16_t flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void Proxy::setup_Tx()
{
    std::string input_address_server;
    std::cout << "Введите адрес куда нужно отправлять: ";
    std::cin >> input_address_server;

    if (inet_pton(AF_INET, input_address_server.c_str(), &address_Tx) < 0)
    {
        std::cerr << "Error inet_pton Server" << std::endl;
    }

    settings_socket(&Tx_addr, address_Tx, port_Tx);

    if (connect(fdTx, reinterpret_cast<sockaddr*>(&Tx_addr), sizeof(Tx_addr)) < 0)
    {
        std::cerr << "Error connecting to Tx" << std::endl;
    }
    set_nonblockfd(fdTx);
}

void Proxy::setup_Rx()
{
    std::string addrRx = "192.168.5.255";
    if (inet_pton(AF_INET, addrRx.c_str(), &address_Rx) < 0)
    {
        std::cerr << "Error inet_pton Server" << std::endl;
    }

    settings_socket(&Rx_addr, address_Rx, port_Rx);   

    if (bind(fdRx, reinterpret_cast<sockaddr*>(&Rx_addr), sizeof(Rx_addr)) < 0)
    {
        std::cerr << "Error binding clientfd" << std::endl;
    }

    set_nonblockfd(fdRx);
}

void Proxy::forward_data()
{
    std::vector<uint8_t> buffer(1024);

    while (true)
    {
        ssize_t bytes = recvfrom(fdRx, buffer.data(), buffer.size(), 0, nullptr, nullptr);
        if (bytes <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            perror("recvfrom");
            continue;
        }

        ssize_t sent = sendto(fdTx, buffer.data(), bytes, 0,
                              reinterpret_cast<sockaddr*>(&Tx_addr), sizeof(Tx_addr));
        if (sent < 0)
        {
            perror("sendto");
            std::cerr << "Error sending data" << std::endl;
        }

        std::cout << "Forwarded " << sent << " bytes:" << std::endl;
        for (ssize_t i = 0; i < bytes; ++i)
        {
            std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
        }
        std::cout << std::dec << std::endl;
    }
}

void Proxy::run()
{
    setup_Rx();
    setup_Tx();
    forward_data();
}