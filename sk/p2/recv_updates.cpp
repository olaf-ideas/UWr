#include "table.hpp"

void RoutingTable::recv_updates() {
    time_t now = time(0);

    struct pollfd ps;
    ps.fd = sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;  
    
    struct sockaddr_in sender;
    socklen_t          sender_len = sizeof(sender);
    uint8_t            buffer[IP_MAXPACKET];

    do {
        int ready = poll(&ps, 1, (TURN_TIME - difftime(time(0), now)) * 1000);
        if (ready < 0) {
            perror("poll error");
            exit(EXIT_FAILURE);
        }

        if (ready > 0) {
            for (;;) {
                ssize_t packet_len = recvfrom(
                    sock_fd,
                    buffer,
                    IP_MAXPACKET,
                    MSG_DONTWAIT,
                    (struct sockaddr*) &sender,
                    &sender_len
                );

                if (packet_len < 0) {
                    if (errno != EWOULDBLOCK) {
                        perror("recvfrom error");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                char sender_ip[INET_ADDRSTRLEN];
                if (inet_ntop(
                    AF_INET,
                    &sender.sin_addr,
                    sender_ip,
                    sizeof(sender_ip)
                ) != 0) {
                    perror("inet_ntop error");
                    exit(EXIT_FAILURE);
                }

                apply_update(sender_ip, (uint8_t*) buffer);
            }
        }
    } while (difftime(time(0), now) < TURN_TIME);
}