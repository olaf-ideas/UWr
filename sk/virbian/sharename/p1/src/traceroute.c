#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "ensure.h"
#include "icmp_send.h"
#include "icmp_recv.h"

#define PACKETS_SEND 3

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <ip address>\n", argv[0]);
    return EXIT_FAILURE;
  }

  ensure(getuid() == 0, "program require root access");

  int sockfd = socket(
    AF_INET, 
    SOCK_RAW, 
    IPPROTO_ICMP
  );
  ensure(sockfd > 0, "socket error");

  struct sockaddr_in recipient;
  memset(&recipient, 0, sizeof(recipient));
  recipient.sin_family = AF_INET;

  ensure(inet_pton(
    AF_INET,
    argv[1],
    &recipient.sin_addr
  ) == 1, "IP address is not correct");

  int found_recipient = 0;
  for (int ttl = 1; ttl <= 30 && !found_recipient; ttl++) {
    printf("%d. ", ttl);

    for (int i = 0; i < PACKETS_SEND; i++) {
      send_icmp_echo(
        sockfd,
        &recipient,
        ttl,
        ttl * PACKETS_SEND + i
      );
    }

    char sender_ip_str[PACKETS_SEND][20];
    int sum_time_ms;

    int recv_packets = receive_icmp_echo(
      sockfd,
      sender_ip_str,
      &sum_time_ms,
      &found_recipient,
      ttl,
      PACKETS_SEND
    );

    for (int i = 0; i < recv_packets; i++) {
      int unique_ip = 1;
      for (int j = 0; j < i; j++) {
        if (!strcmp(sender_ip_str[i], sender_ip_str[j])) {
          unique_ip = 0;
          break;
        }
      }

      if (unique_ip) {
        printf("%s ", sender_ip_str[i]);
      }
    }

    if (recv_packets == 0) {
      puts("*");
    }
    else if (recv_packets < PACKETS_SEND) {
      puts("???");
    }
    else {
      printf("%dms\n", sum_time_ms / PACKETS_SEND);
    }
  }

  close(sockfd);

  return EXIT_SUCCESS;
}