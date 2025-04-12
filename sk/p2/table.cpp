#include "table.hpp"

RoutingTable::RoutingTable() {
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int broadcast = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(54321);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
}

RoutingTable::~RoutingTable() {
    close(sock_fd);
}

std::istream& operator>>(std::istream &is, RoutingTable& rt) {
    int interface_count;
    is >> interface_count;

    for (int i = 0; i < interface_count; i++) {
        std::string network, _;
        uint32_t distance;

        is >> network >> _ >> distance;

        std::cerr << "? " << network << ' ' << distance << '\n';

        std::string network_ip_str = network.substr(0, network.find('/'));
        uint8_t network_mask = stoi(network.substr(network.find('/') + 1));

        struct in_addr addr;
        inet_pton(AF_INET, network_ip_str.c_str(), &addr);

        uint32_t mask = ~(((1LL << (32 - network_mask)) - 1));
        uint32_t network_addr = htonl(ntohl(addr.s_addr) | mask);

        char broadcast_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr, broadcast_ip, INET_ADDRSTRLEN);

        rt.interfaces.push_back(RoutingTable::Interface {
            network,
            network_addr,
            network_mask,
            distance,
            std::string(broadcast_ip)
        });

        rt.routing_table[network] = RoutingTable::Entry {
            network_addr,
            network_mask,
            distance,
            "<direct connection>",
            true,
            true,
            time(0)
        };
    }

    return is;
}

std::ostream& operator<< (std::ostream& os, RoutingTable const& rt) {
    for (auto const& [network, entry] : rt.routing_table) {
        os << network << ' ';
        if (entry.is_reachable) {
            os << "distance " << entry.network_dist << ' ';
        }
        else {
            os << "unreachable ";
        }

        if (entry.is_direct) {
            os << "connected directly";
        }
        else 
        if (entry.is_reachable) {
            os << "via " << entry.next_hop;
        }

        os << std::endl;
    }

    return os;
}

void RoutingTable::check_timeouts() {
    for (auto it = routing_table.begin(); it != routing_table.end(); ) {
        Entry& entry = it->second;

        if (difftime(time(0), entry.last_update) > TURN_TIME * 3) {
            // std::cerr << "unreachable: " << it->first << '\n';
            if (entry.network_dist != INFINITY_DISTANCE) {
                entry.network_dist = INFINITY_DISTANCE;
                entry.is_reachable = false;
                entry.last_update = time(0);
                it++;
            }   
            else 
            if (!entry.is_direct) {
                it = routing_table.erase(it);
            }
            else {
                it++;
            }
        }
        else {
            it++;
        }
    }
}

