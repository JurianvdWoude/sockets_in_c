#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {
    int x;
    const char *addr[] = {
        "44.135.86.12",
        "127.0.0.1",
        "172.16.23.95",
        "192.168.9.1"
    };
    unsigned long net_addr;

    // convert dottedquad IP to 32bit host-ordered value
    for ( x = 0; x < 4; ++x ) {
        net_addr = inet_network(addr[x]);
        printf(
            "%14s = 0x%08X net 0x%X\n",
            addr[x],
            (int)net_addr,
            (unsigned int)htonl(net_addr)
        );
        
    }
        return 0;
}
