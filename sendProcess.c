#include "sendProcess.h"
#include "threadcancel.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo *serverhints;

static List* sendList;
static pthread_mutex_t *sendMutex;

static pthread_t sendThread;

static int sendSocket;

static char* theirPort;

static char*hostname;

pthread_cond_t *sendCondition;

void* send_input(void* arg) {
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    int numbytes;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(hostname, theirPort, &hints, &serverInfo) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    char* message;
    // Loop for sending
    while (1) {
        pthread_mutex_lock(sendMutex);
        {
            pthread_cond_wait(sendCondition, sendMutex);
            List_first(sendList);
            message = List_remove(sendList);
        }
        pthread_mutex_unlock(sendMutex);

        numbytes = sendto(sendSocket, message, strlen(message), 0, serverInfo->ai_addr, serverInfo->ai_addrlen);
        if (!strcmp(message, "!\n")) {
            free(message);
            message = NULL;
            freeaddrinfo(serverInfo);
            return NULL;
        }
        free(message);
        message = NULL;
    }

    freeaddrinfo(serverInfo);

    return NULL;
}


void * send_createThread(char* host, char* port, int socket, List* list2, pthread_mutex_t *mutex, pthread_cond_t *condition){
    sendList = list2;
    sendMutex = mutex;
    sendCondition = condition;
    sendSocket = socket;
    //sprintf(theirPort,"%d",port);
    theirPort = port;
    hostname = host;
    pthread_create(&sendThread,NULL,send_input,NULL);
    sendCancelInit(sendThread);
}

void* send_joinThread() {
    pthread_join(sendThread, NULL);
}