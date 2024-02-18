#include "sendProcess.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo *serverinfo;

static List* sendList;
static pthread_mutex_t *sendMutex;

static pthread_t sendThread;

static int sendSocket;

static char* theirPort;

static char*hostname;

pthread_cond_t *sendCondition;

void* send_input(void* arg) {
    struct addrinfo info,*p;
    int getAddr;
    int numbytes;

    memset(&info, 0, sizeof(info));
    info.ai_family = PF_INET;
    info.ai_socktype = SOCK_DGRAM;

    getAddr = getaddrinfo(hostname, theirPort, &info, &p);

    // Create and bind socket
    int socketID = -1;
    for (struct addrinfo* temp = p; temp != NULL; temp = temp->ai_next) {
        socketID = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);

        // Check for socket creation errors here
        
        // You might want to bind the socket here as well
        bind(socketID, temp->ai_addr, temp->ai_addrlen);
    }
    char* message;
    // Loop for sending
    while (1) {
        pthread_mutex_lock(sendMutex);
        {
            pthread_cond_wait(sendCondition,sendMutex);
            List_first(sendList);
            printf("Message Sent\n");
            message = List_remove(sendList);
        }
        pthread_mutex_unlock(sendMutex);

        int count = 0;
        numbytes = sendto(socketID, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);
        if (!strcmp(message, "!\n") && count == 1) {
                free(message);
                message = NULL;
                return NULL;
            }
        free(message);
        message = NULL;
    }

    return NULL;
}

void * send_createThread(char* host, char* port, List* list2, pthread_mutex_t *mutex, pthread_cond_t *condition){
    sendList = list2;
    sendMutex = mutex;
    sendCondition = condition;

    //sprintf(theirPort,"%d",port);
    // theirPort = port;

    hostname = host;
    pthread_create(&sendThread,NULL,send_input,NULL);


}

void* send_joinThread() {
    pthread_join(sendThread, NULL);
}