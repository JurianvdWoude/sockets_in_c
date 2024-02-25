#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void
bail(const char *str) {
    fprintf(stderr, "%s: %s\n", strerror(errno), str);
    exit(1);
}

int
main(int argc, char **argv) {
    int z;                          // status code
    int s_fd;                       // socket fd
    struct sockaddr_in s_addr;      // socket address
    socklen_t s_naddr_len;          // retrieving socket address length
    struct sockaddr_in s_naddr;     // retrieving socket address
    
    printf("creating socket\n");
    s_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( s_fd < 0 ) bail("socket()");

    // clear socket address
    memset(&s_addr, 0, sizeof s_addr);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(9000);

    z = inet_aton("127.0.0.24", &s_addr.sin_addr);
    if ( z < 0 ) bail("inet_aton()");
 
    // bind address to socket
    printf("binding to socket\n");
    z = bind(s_fd, (struct sockaddr *)&s_addr, sizeof s_addr);
    if ( z < 0 ) bail("bind()");

    // get the socket address from a file descriptor
    s_naddr_len = sizeof s_naddr;
    z = getsockname(s_fd, (struct sockaddr *)&s_naddr, &s_naddr_len);
    if ( z < 0 ) bail("getsockname()");

    printf("address: %s, port: %u\n", inet_ntoa(s_naddr.sin_addr), ntohs(s_naddr.sin_port));

    system("netstat -pa --tcp 2>/dev/null | grep 9000");

    printf("closing socket\n");
    close(s_fd);
    return 0;
}
