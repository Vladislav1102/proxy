#include "proxy.hpp"

namespace
{
    uint16_t open_socket()
    {
        uint8_t optval = 1;
        uint16_t new_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (new_socket <= 0)
        {
            std::cerr << "Error open socket" << std::endl;
            return EXIT_FAILURE;
        }

        if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(uint16_t)) < 0)
        {
            std::cerr << "Error set socket" << std::endl;
            return EXIT_FAILURE;
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
} //namespace

Proxy::Proxy()
{
    fdRx = ::open_socket();
    fdTx = ::open_socket();
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
    std::cout << "Введите адрес куда нужно отправить: ";
    std::cin >> input_address_server;

    if (inet_pton(AF_INET, input_address_server.c_str(), &address_Tx) < 0)
    {
        std::cerr << "Error inet_pton Server" << std::endl;
    }

    ::settings_socket(&Tx_addr, address_Tx, port_Tx);

    if (connect(fdTx, reinterpret_cast<sockaddr*>(&Tx_addr), sizeof(Tx_addr)) < 0)
    {
        std::cerr << "Error connecting to Tx" << std::endl;
    }
}

void Proxy::setup_Rx()
{
    set_nonblockfd(fdRx);
    ::settings_socket(&Rx_addr, address_Rx, port_Rx);   

    if (bind(fdRx, reinterpret_cast<sockaddr*>(&Rx_addr), sizeof(Rx_addr)) < 0)
    {
        std::cerr << "Error binding clientfd" << std::endl;
    }
    if (listen(fdRx, SOMAXCONN) < 0)
    {
        std::cerr << "Error listening" << std::endl;
    }
}

void Proxy::forward_data()
{
    std::vector<uint8_t>buffer;

    bool flag = true;
    while (flag)
    {
        ssize_t bytes = read(fdTx, &buffer, sizeof(buffer));
        if (bytes <= 0) 
        {
            flag = false;
        }

        ssize_t sendData = sendto(fdTx, &buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&Tx_addr), sizeof(Tx_addr));
        if (sendData == -1)
        {
            std::cerr << "Error sending data" << std::endl;
        }

        for (int i : buffer)
        {
            std::cout << i << std::endl;
        }
    }
}