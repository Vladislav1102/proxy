#ifndef PROXY_HPP
#define PROXY_HPP

#include <iostream>
#include <memory>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <sys/epoll.h>

static constexpr uint16_t port_client = 9000;
static constexpr uint16_t port_server = 1234;
static constexpr uint16_t MAX_BUFFER  = 4096;

class InterfaceServer
{
public:
    virtual void handler_data() = 0;
    virtual void setup()        = 0;
    virtual ~InterfaceServer()  = default;

    struct sockaddr_in client_addr, server_addr;
};

class RealServer : public InterfaceServer 
{
public:
     RealServer();
    ~RealServer();

    void setup() override;
    void handler_data() override;

private:
    uint16_t serverfd;
    in_addr_t address_server;
    std::string input_address_server;
};

class Proxy : public InterfaceServer
{
public:
     Proxy(InterfaceServer* server) : real_server(server) {};
    ~Proxy();

    void setup() override;
    void handler_data() override;

    void init_epoll();
    void handle_connecting_client();

private:
    uint16_t clientfd;
    in_addr_t address_client = INADDR_ANY;

    void connect_client_to_server();

    std::unique_ptr<InterfaceServer> real_server;
};

uint16_t open_socket();
void settings_socket(struct sockaddr_in*, in_addr_t& address, int16_t port);
void run(const InterfaceServer*);

#endif //PROXY_HPP