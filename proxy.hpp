#ifndef PROXY_HPP
#define PROXY_HPP

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>


static constexpr uint16_t port = 9000;
static constexpr uint16_t MAX_SIZE = 4096;

class Proxy {
    
public:
    Proxy();
    ~Proxy();
    void run_proxy();

private:
    uint16_t open_socket();
    uint16_t setup_host();
    uint16_t setup_target();
    void handler(uint16_t socket_host, uint16_t socket_target);
    void signal_handler();

    struct sockaddr_in host, target;

    uint8_t opt = 1;
    uint16_t socketfd;
};

#endif //PROXY_HPP