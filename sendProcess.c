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

struct addrinfo *sendInfo;

void* send_input(void* arg) {
    int numbytes;

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

        numbytes = sendto(sendSocket, message, strlen(message), 0, sendInfo->ai_addr, sendInfo->ai_addrlen);
        if (strcmp(message, "!\n")==0) {
            message = NULL;
            break;
        }
        message = NULL;
        
    }
    return NULL;
}


void * send_createThread(char* host, char* port, int socket, List* list2, 
pthread_mutex_t *mutex, pthread_cond_t *condition, struct addrinfo *serverInfo){
    sendList = list2;
    sendMutex = mutex;
    sendCondition = condition;
    sendSocket = socket;
    //sprintf(theirPort,"%d",port);
    theirPort = port;
    hostname = host;
    sendInfo = serverInfo;
    pthread_create(&sendThread,NULL,send_input,NULL);
    sendCancelInit(sendThread);
}

void* send_joinThread() {
    pthread_join(sendThread, NULL);
}