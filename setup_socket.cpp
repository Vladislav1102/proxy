#include "proxy.hpp"

uint16_t open_socket()
{
    uint8_t optval = 1;
    uint16_t new_socket = socket(AF_INET, SOCK_STREAM, 0);
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
    device->sin_family      = AF_INET;
    device->sin_addr.s_addr = address;
    device->sin_port        = htons(port);
}