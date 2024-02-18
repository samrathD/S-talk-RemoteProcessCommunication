#include "sendProcess.h"
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
    int getAddr;
    int numbytes;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if(getaddrinfo(hostname, theirPort, &hints, &serverInfo)!=0){
        exit(-1);
    }


    // Create and bind socket
    // int socketID = -1;
    // struct addrinfo* temp = serverInfo; 
    // temp != NULL; 
    // temp = temp->ai_next;
    // socketID = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

        // Check for socket creation errors here
        // You might want to bind the socket here as well
       
       // bind(socketID, serverInfo->ai_addr, serverInfo->ai_addrlen);

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
        numbytes = sendto(sendSocket, message, strlen(message), 0, serverInfo->ai_addr, serverInfo->ai_addrlen);
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

void * send_createThread(char* host, char* port, int socket, List* list2, pthread_mutex_t *mutex, pthread_cond_t *condition){
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