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
#include <poll.h>

static constexpr uint16_t port = 9000;
static constexpr uint16_t MAX_SIZE = 4;
static constexpr uint16_t port_target = 1234;

class Proxy {
    
public:
    Proxy();
    ~Proxy();
    void run_proxy();

private:
    int32_t open_socket();
    void setup_client();
    void setup_target();
    void transfer_data();
    void install_settings_sockets(struct sockaddr_in* device, in_addr_t& address, uint16_t port);
    void connect_client_to_server();
    void signal_handler();

    struct sockaddr_in client_addr, target_addr;

    in_addr_t address_client = INADDR_ANY;
    in_addr_t address_target;

    uint8_t opt = 1;
    int32_t socket_client;
    int32_t socket_target;
    int32_t connect_client;
};

#endif //PROXY_HPP