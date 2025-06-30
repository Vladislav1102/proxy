#ifndef PROXY_HPP
#define PROXY_HPP

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <vector>

static constexpr uint16_t port_Rx = 9000;
static constexpr uint16_t port_Tx = 1234;

class Proxy
{
public:
    Proxy();
    ~Proxy();

    void run();
private:
    sockaddr_in Rx_addr, Tx_addr;
    
    in_addr_t address_Tx {0};
    in_addr_t address_Rx = {0};

    int16_t fdRx, fdTx;
    
    void setup_Tx();
    void setup_Rx();
    void accept_connection();
    void forward_data();
    void set_nonblockfd(int16_t fd);
};

#endif //PROXY_HPP