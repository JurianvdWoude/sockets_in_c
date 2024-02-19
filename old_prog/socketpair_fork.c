/* Listing 1.3
*
* Client/Server Example Using socketpair(2)
* and fork(2):
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

/*
* As of RedHat-6.0, these are still not defined:
*/
#ifndef SHUT_WR
#define SHUT_RD     0
#define SHUT_WR     1
#define SHUT_RDWR   2
#endif

void bail(const char *msg) {
    fprintf(stderr, "%s: %s", strerror(errno), msg);
    exit(1);
}

/*
* Main program: building a socket
*/
int main(int argc,char **argv) {
    int z;                      // Status return code
    int s[2];                   // Pair of sockets
    char *msgp;                 // A message pointer
    int mlen;                   // Message length
    char buf[80];               // Work buffer
    pid_t chpid;                // Child PID

    z = socketpair(AF_UNIX, SOCK_STREAM, 0, s);

    if ( z == -1 ) {
        bail("socketpair(AF_UNIX, SOCK_STREAM, 0, s)");
    }

    // Fork into two processes
    if ( (chpid = fork()) == (pid_t)-1) {
        bail("fork()");
    } else if ( chpid == 0 ) {
        // this is the child process
        char rxbuf[80];         // Receive buffer

        printf("Parent PID is %d\n", getppid());

        close(s[0]);
        s[0] = -1;

        msgp = "%A %d-%b-%Y %l:%M %p";
        mlen = strlen(msgp);

        printf("Child sending request '%s'\n", msgp);
        fflush(stdout);

        // Write a request to the Server
        z = write(s[1], msgp, mlen);
        if ( z == -1 ) {
            bail("68, write(s[1], msgp, mlen)");
        }

        if ( shutdown(s[1], SHUT_WR) == -1 ) {
            bail("72, shutdown(s[1], SHUT_WR)");
        }

        z = read(s[1], rxbuf, sizeof rxbuf);
        if ( z == -1 ) {
            bail("77, read(s[1], rxbuf, sizeof rxbuf)");
        }

        rxbuf[z] = 0;

        printf("Parent returned '%s'\n", rxbuf);
        fflush(stdout);
        close(s[1]);
    } else {
        // this is the parent process
        int status;             // Child termination status
        char txbuf[80];         // Reply buffer
        time_t td;              // Current date and time

        printf("Child PID is %ld\n", (long)chpid);
        fflush(stdout);

        close(s[1]);
        s[1] = -1;

        z = read(s[0], buf, sizeof buf);
        if ( z == -1 ) {
            bail("97, read(s[0], buf, sizeof buf)");
        }

        printf("Child sent value: %s\n", buf);
        fflush(stdout);

        buf[z] = 0;
        
        time(&td);              // Get the current time
        
        strftime(
            txbuf,              // Buffer
            sizeof txbuf,       // Buffer size
            buf,                // Format
            localtime(&td)      // Local time
        );

        z = write(s[0], txbuf, strlen(txbuf));
        if ( z == -1 ) {
            bail("113, write(s[1], txbuf, strlen(txbuf))");
        }

        close(s[1]);

        waitpid(chpid, &status, 0);
    }

    return 0;
}
