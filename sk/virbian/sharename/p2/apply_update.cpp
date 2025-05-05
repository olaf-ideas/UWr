// Olaf Surgut 345615
#include "table.hpp"

void RoutingTable::apply_update(struct sockaddr_in& sender,
                                  uint8_t* const packet) {


    char sender_ip[INET_ADDRSTRLEN];
    if (inet_ntop(
        AF_INET,
        &sender.sin_addr,
        sender_ip,
        sizeof(sender_ip)
    ) == 0) {
        perror("inet_ntop error");
        exit(EXIT_FAILURE);
    }

    struct in_addr addr;
    inet_pton(AF_INET, sender_ip, &addr);
    uint32_t sender_addr = addr.s_addr;

    uint32_t network_addr;
    uint8_t  network_mask;
    uint32_t network_dist;
    memcpy(&network_addr, packet + 0, 4);
    memcpy(&network_mask, packet + 4, 1);
    memcpy(&network_dist, packet + 5, 4);
    
    addr.s_addr = network_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));

    std::string network = std::string(ip_str) + "/" + std::to_string(network_mask);
    auto it = routing_table.find(network);

    for (auto& interface : interfaces) {
        if ((ntohl(interface.network_addr) >> interface.network_mask) ==
            (ntohl(sender_addr) >> interface.network_mask)) {
            interface.last_entries[sender_ip] = time(0);
        }
    }

    // std::cerr << "packet received from: " << sender_ip << '\n';
    // std::cerr << " > " << network << " " << network_dist << '\n';

    if (it == routing_table.end()) {
        if (network_dist < INFINITY_DISTANCE) {
            routing_table[network] = Entry {
                network_addr,
                network_mask,
                network_dist,
                std::string(sender_ip),
                false,
                time(0),
            };
        }
    }
    else {
        Entry& entry = it->second;

        if (entry.next_hop == sender_ip) {
            entry.network_dist = network_dist;
            if (network_dist != INFINITY_DISTANCE) {
                entry.last_update = time(0);
            }
        }
        else
        if (entry.network_dist > network_dist) {
            entry.network_dist = network_dist;
            entry.next_hop = sender_ip;
            entry.last_update = time(0);
        }
    }
}