#include "table.hpp"

void RoutingTable::apply_update(std::string const& sender_ip,
                                  uint8_t* const packet) {
    uint32_t network_addr;
    uint8_t  network_mask;
    uint32_t network_dist;
    memcpy(&network_addr, packet + 0, 4);
    memcpy(&network_mask, packet + 4, 1);
    memcpy(&network_dist, packet + 5, 4);

    struct in_addr addr;
    addr.s_addr = network_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));

    std::string network = std::string(ip_str) + "/" + std::to_string(network_mask);
    auto it = routing_table.find(network);

    if (network_dist == INFINITY_DISTANCE) {
        return;
    }

    if (it == routing_table.end()) {
        routing_table[network] = Entry {
            network_addr,
            network_mask,
            network_dist,
            sender_ip,
            false,
            true,
            time(0),
        };
    }
    else {
        Entry& entry = it->second;

        if (entry.next_hop == sender_ip) {
            entry.network_dist = network_dist;
            entry.last_update = time(0);
            entry.is_reachable = true;
        }
        else
        if (entry.network_dist > network_dist) {
            entry.network_dist = network_dist;
            entry.next_hop = sender_ip;
            entry.last_update = time(0);
            entry.is_reachable = true;
        }
    }
}