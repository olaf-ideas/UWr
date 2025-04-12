#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>

#define TURN_TIME 5
#define INFINITY_DISTANCE 100

class RoutingTable {

    struct Entry {
        uint32_t network_addr;
        uint8_t network_mask;
        uint32_t network_dist;

        std::string next_hop;

        bool is_direct;
        bool is_reachable;

        time_t last_update;
    };

    struct Interface {
        std::string network;
        uint32_t network_addr;
        uint8_t  network_mask;
        uint32_t network_dist;

        std::string broadcast_ip;
    };

private:

    std::map<std::string, Entry> routing_table;
    std::vector<Interface> interfaces;

    int sock_fd;

public:

    RoutingTable();
    ~RoutingTable();

    void read_config();

    void recv_updates();
    void send_updates();

    void check_timeouts();

    void apply_update(std::string const&, uint8_t* const);

    friend std::istream& operator>> (std::istream&, RoutingTable&);
    friend std::ostream& operator<< (std::ostream&, RoutingTable const&);
};

