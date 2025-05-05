// Olaf Surgut 345615
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

#define __USE_MISC
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <poll.h>

#include "ensure.h"

#define TIMEOUT 1000

int elapsed_time(struct timeval* start, struct timeval* now) {
  return (now->tv_sec - start->tv_sec) * 1000 + (now->tv_usec - start->tv_usec) / 1000;
}

int receive_icmp_echo(int sockfd, char sender_ip_str[][20], int* sum_time_ms, int* found_recipient, int ttl, int packets_send) {

  struct timeval start, now;
  ensure(gettimeofday(&now, 0) == 0, "gettimeofday error");
  start = now;

  int recv_packets = 0;

  struct pollfd ps;
  ps.fd = sockfd;
  ps.events = POLLIN;
  ps.revents = 0;  

  struct sockaddr_in sender;
  socklen_t          sender_len = sizeof(sender);
  uint8_t            buffer[IP_MAXPACKET];

  do {
    int ready = poll(&ps, 1, TIMEOUT - elapsed_time(&start, &now));
    ensure(ready >= 0, "poll error");
    ensure(gettimeofday(&now, 0) == 0, "gettimeofday error");

    if (ready > 0) {
      ensure(ps.revents == POLLIN, "poll error");

      for (;;) {

        ssize_t packet_len = recvfrom(
          sockfd,
          buffer,
          IP_MAXPACKET,
          MSG_DONTWAIT,
          (struct sockaddr*) &sender,
          &sender_len
        );

        if (packet_len < 0) {
          ensure(errno == EWOULDBLOCK, "recvfrom error");
          break;
        }

        ensure(inet_ntop(
          AF_INET,
          &(sender.sin_addr),
          sender_ip_str[recv_packets],
          sizeof(sender_ip_str[recv_packets])
        ) != 0, "inet_ntop error");

        struct ip*   ip_header = (struct ip*) buffer;
        uint8_t*     icmp_packet = buffer + 4 * ip_header->ip_hl;
        struct icmp* icmp_header = (struct icmp*) icmp_packet;

        if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED) {
          ip_header = (struct ip*) (icmp_packet + 8);
          icmp_header = (struct icmp*) (icmp_packet + 8 + 4 * ip_header->ip_hl);
        }

        if (icmp_header->icmp_hun.ih_idseq.icd_id == getpid() &&
            icmp_header->icmp_hun.ih_idseq.icd_seq / packets_send == ttl) {
          if (icmp_header->icmp_type == ICMP_ECHOREPLY) {
            *found_recipient = 1;
          }

          *sum_time_ms += elapsed_time(&start, &now);
          recv_packets++;
        }
      }
    }
  } while (recv_packets < packets_send && 
          elapsed_time(&start, &now) < TIMEOUT);
  
  return recv_packets;
}