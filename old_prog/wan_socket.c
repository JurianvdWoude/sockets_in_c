#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/x25.h>


static void bail(const char *str) {
    perror(str);
    exit(1);
}

int main(int argc, char **argv, char **envp) {
    int z;                          // status code
    int sck_x25;                    // socket
    struct sockaddr_x25 adr_x25;    // AF_X25
    int len_x25;                    // length
    const char x25_host[]           // X.121 addr
        = "79400900";

    sck_x25 = socket(AF_X25, SOCK_SEQPACKET, 0);

    if ( sck_x25 == -1 ) {
        bail("socket()");
    }

    adr_x25.sx25_family = AF_X25;
    strcpy(adr_x25.sx25_addr.x25_addr, x25_host);
    len_x25 = sizeof adr_x25;

    z = bind(sck_x25, (struct sockaddr *) &adr_x25, len_x25);

    if ( z == -1 ) {
        bail("bind()");
    }

    puts("X.25 SOCKETS :");
    system("cat /proc/net/x25");
    return 0;
}
