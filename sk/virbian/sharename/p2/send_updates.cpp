// Olaf Surgut 345615
#include "table.hpp"

void RoutingTable::send_updates() {
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(54321);

    for (auto const& interface : interfaces) {
        // std::cerr << "broadcast: " << interface.broadcast_ip.c_str() << std::endl;        
        inet_pton(AF_INET, interface.broadcast_ip.c_str(), &dest_addr.sin_addr);

        for (auto& [network, entry] : routing_table) {
            uint32_t true_distance = entry.network_dist + interface.network_dist;
            if (true_distance > INFINITY_DISTANCE) {
                true_distance = INFINITY_DISTANCE;
            }

            // std::cerr << "sending: " << true_distance << '\n';

            uint8_t packet[9];
            memcpy(packet + 0, &entry.network_addr, 4);
            memcpy(packet + 4, &entry.network_mask, 1);
            memcpy(packet + 5, &true_distance, 4);

            if (sendto(
                sock_fd, 
                packet, 
                sizeof(packet), 
                0,
                (struct sockaddr*) &dest_addr, 
                sizeof(dest_addr)
            ) != 9) {
				// entry.network_dist = INFINITY_DISTANCE;
                // std::cerr << "interface failed!!!\n";
            }
        }
    }
}
