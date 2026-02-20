#include "csapp.h"

int main(int argc, char **argv) {
  struct addrinfo *p, *listp, hints;
  // char buf[MAXLINE];
  int rc; //flags;

  if (argc != 2 && argc != 3)
    app_error("usage: %s <domain name> <optional service>\n", argv[0]);

  /* Get a list of addrinfo records */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC; /* IPv4 + IPv6 */
  // aby tftp działo to musi być tutaj SOCK_DGRAM ponieważ działą na UDP
  hints.ai_socktype = SOCK_STREAM;
  /* Connections only */
  if ((rc = getaddrinfo(argv[1], argv[2], &hints, &listp)) != 0)
    gai_error(rc, "getaddrinfo");

  /* Walk the list and display each IP address */
  // flags = NI_NUMERICHOST; /* Display address string instead of domain name */
  for (p = listp; p; p = p->ai_next) {
    char ip[INET6_ADDRSTRLEN];
    void *addr = NULL;
    int port = 0;

    if (p->ai_family == AF_INET) {
      struct sockaddr_in *sin = (struct sockaddr_in *) p->ai_addr;
      addr = &sin->sin_addr;
      port = ntohs(sin->sin_port);
      assert(inet_ntop(AF_INET, addr, ip, sizeof(ip)));
      printf("%s", ip);
    } else if (p->ai_family == AF_INET6) {
      struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) p->ai_addr;
      addr = &sin6->sin6_addr;
      port = ntohs(sin6->sin6_port);
      assert(inet_ntop(AF_INET6, addr, ip, sizeof(ip)));
      printf("[%s]", ip);
    } else {
      continue;
    }

    if (argc == 3)
      printf(":%d", port);
    puts("");
  }

  /* Clean up */
  freeaddrinfo(listp);

  return EXIT_SUCCESS;
}
