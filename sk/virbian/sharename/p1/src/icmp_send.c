#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define __USE_MISC
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#include "ensure.h"

uint16_t compute_icmp_checksum(const void *buff, int length) {
  const uint16_t* ptr = buff;
  uint32_t sum = 0;
  ensure(length % 2 == 0, "compute_icmp_checksum error");
  for (; length > 0; length -= 2)
    sum += *ptr++;
  sum = (sum >> 16U) + (sum & 0xffffU);
  return (uint16_t)(~(sum + (sum >> 16U)));
}

void send_icmp_echo(int sockfd, struct sockaddr_in* recipient, int ttl, int seq) {

  struct icmp header;

  header.icmp_type = ICMP_ECHO;
  header.icmp_code = 0;
  header.icmp_hun.ih_idseq.icd_id = getpid();
  header.icmp_hun.ih_idseq.icd_seq = seq;
  header.icmp_cksum = 0;
  header.icmp_cksum = compute_icmp_checksum(
    (uint16_t*) &header, sizeof(header)
  );

  ensure(setsockopt(
    sockfd,
    IPPROTO_IP,
    IP_TTL,
    &ttl,
    sizeof(int)
   ) >= 0, "setsockopt error"
  );
  
  ensure(sendto(
    sockfd,
    &header,
    sizeof(header),
    0,
    (struct sockaddr*) recipient,
    sizeof(*recipient)
  ) > 0, "sendto error");
}
