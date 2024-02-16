#include "receiveProcess.h"

#define PORT 22110
#define MSG_MAX_LEN 1024

static int socketDescriptor = -1;
static List* receiveList;
static pthread_mutex_t receiveMutex;

static pthread_t receiveThread;

int my_port;

void* receive_input(void* unused) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    // Create udp socket with error check
    if ((socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("receive_input: socket error");
        // Handle error, maybe exit or return
        return NULL;
    }

    // Bind with error check
    if (bind(socketDescriptor, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
        perror("receive_input: bind error");
        close(socketDescriptor);
        // Handle error, maybe exit or return
        return NULL;
    }

    while (1) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];

        memset(messageRx, 0, sizeof(messageRx));
        int bytesRx = recvfrom(socketDescriptor, messageRx, MSG_MAX_LEN, 0,
                               (struct sockaddr*)&sinRemote, &sin_len);

        // Error check recvfrom
        if (bytesRx == -1) {
            perror("receive_input: recvfrom error");
            continue; // Skip processing this message, continue with the next iteration
        }

        // Null terminated (string)
        int terminatedIdx = (bytesRx < MSG_MAX_LEN) ? bytesRx : MSG_MAX_LEN - 1;
        messageRx[terminatedIdx] = 0;

        // Lock Mutex
        pthread_mutex_lock(&receiveMutex);
        {
            List_append(receiveList, messageRx);
        }
        pthread_mutex_unlock(&receiveMutex);

        printf("message received: %s\n", messageRx);
    }
}

void* receive_createThread(List* list2, char* port, pthread_mutex_t mutex) {
    receiveList = list2;
    receiveMutex = mutex;

    // Convert port from string to integer
    // Assign port to my_port
    my_port = atoi(port);

    // Create the receive thread with error check
    if (pthread_create(&receiveThread, NULL, receive_input, NULL) != 0) {
        perror("receive_createThread: pthread_create error");
        // Handle error, maybe exit or return
        return NULL;
    }
}

void* receive_joinThread() {
    pthread_join(receiveThread, NULL);
}
