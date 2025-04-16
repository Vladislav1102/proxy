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
    void setup_host();
    void setup_target(std::string& input_addres);
    void handler();
    void install_settings_sockets(struct sockaddr_in* device, in_addr_t* address);
    void signal_handler();

    struct sockaddr_in host, target;

    in_addr_t address_host   = 0;
    in_addr_t address_target = 0;

    uint8_t opt = 1;
    uint16_t socket_host;
    uint16_t socket_target;
};

#endif //PROXY_HPP