#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void bail(const char *str) {
    fprintf(stderr, "%s: \n%s", strerror(errno), str);
    exit(1);
}

int main(int argc, char **argv, char **envp) {
    int z;                          // Status code
    int sck_inet;                   // Socket
    int len_inet;                   // Address length
    struct sockaddr_in adr_inet;    // AF_INET
    const unsigned char IPno[]      // Socket address path
    = {
        127, 0, 0, 23
    };

    sck_inet = socket(AF_INET, SOCK_STREAM, 0);

    if ( sck_inet < 0 ) {
        bail("socket()");
    }

    memset(&adr_inet, 0, sizeof adr_inet);

    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(9000);
    memcpy(&adr_inet.sin_addr.s_addr, IPno, 4);
    len_inet = sizeof adr_inet;
    
    z = bind(sck_inet, (struct sockaddr *) &adr_inet, len_inet);

    if ( z < 0 ) {
        bail("bind()");
    }

    // Display all of our bound sockets
    system("netstat -pa --tcp 2>/dev/null | sed -n '1,/^Proto/p;af_inet/p'");

    close(sck_inet);
    return 0;
}
