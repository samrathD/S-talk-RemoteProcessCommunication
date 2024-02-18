#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "list.h"

void* send_process(void* arg);

// Define a struct to hold thread parameters
// Define a struct to hold thread parameters
struct threadParameters {
    List* list;
    pthread_mutex_t mutex;
    int socket;
    struct addrinfo *serverInfo;
};
void* send_createThread(List* listToSend, char* destIP, char* destPort, pthread_mutex_t mutex);

void* send_joinThread();