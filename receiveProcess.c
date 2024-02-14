#include "receiveProcess.h"

#define PORT 22110 // Replace
#define MSG_MAX_LEN 1024 // Replacable

static int socketDescriptor = -1;

void* receiveThread(void* unused) {
    // address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    // create udp socket
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
    while (1) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];

        memset(messageRx, 0, sizeof(messageRx));
        recvfrom(socketDescriptor, messageRx, MSG_MAX_LEN, 0, (struct sockaddr*) &sinRemote, &sin_len);

        printf("message received: %s\n", messageRx);
    }
}
