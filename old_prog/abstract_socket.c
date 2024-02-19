#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>


static void bail(const char *str) {
    perror(str);
    exit(1);
}

int main(int argc, char **argv, char **envp) {
    int z;                          // Status return code
    int sck_unix;                   // Socket
    struct sockaddr_un adr_unix;    // AF_UNIX
    int len_unix;                   // Socket length
    const char pth_unix[]           // Abstract name
        = "Z*MY-SOCKET*";

    sck_unix = socket(AF_UNIX, SOCK_STREAM, 0);

    if( sck_unix == -1 ) {
        bail("socket(AF_UNIX, SOCK_STREAM, 0)");
    }

    memset(&adr_unix, 0, sizeof adr_unix);

    adr_unix.sun_family = AF_UNIX;
    strncpy(
        adr_unix.sun_path,
        pth_unix,
        sizeof adr_unix.sun_path - 1
    )[sizeof adr_unix.sun_path - 1] = 0;

    len_unix = SUN_LEN(&adr_unix);

    adr_unix.sun_path[0] = 0;

    z = bind(sck_unix, (struct sockaddr *)&adr_unix, len_unix);

    if ( z == -1 ) {
        bail("bind()");
    }

    system("netstat -pa --unix 2>/dev/null | grep MY-SOCKET");

    close(sck_unix);
    return 0;
}
