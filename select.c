#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include "select.h"
#include <errno.h>
#include "main.h"
#include "macro.h"
#include "socket.h"

void createSelectFd(fd_set *fd, int newfd, int *maxfd){
    pthread_mutex_lock(&selectLock);
    FD_SET(newfd, fd);
    if (newfd > *maxfd) {
        *maxfd = newfd;
    }
    pthread_mutex_unlock(&selectLock);
}

int waitForSelectEvent(fd_set *rdset, fd_set *rwset, int maxfd){
    struct timeval wait;
    wait.tv_sec = 0;
    wait.tv_usec = 10;

    int n;
    if ((n = select(maxfd + 1, rdset, rwset, NULL, &wait)) < 0) {
        if (errno == EINTR) {
            //Interrupted by signal, ignore it
            return 0;
        }
        fatal_error("select wait");
    }
    return n;
}

size_t singleSelectReadInstance(const int sock, unsigned char *buffer, const size_t bufSize, fd_set* fd, int* maxfd ){
    FD_ZERO(fd);
    *maxfd = sock;
    FD_SET(sock, fd);

    size_t n = 0;
    int i;
    //waitForSelectEvent(fd, maxfd);
    int max = *maxfd;
    for(i = 0; i <= max ; i++){
        if(FD_ISSET(i, fd)){
            if(i == sock){

            } else {
                n = readNBytes(sock, buffer, bufSize);
            }
        }
    }
    return n;
}
