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
struct threadParameters {
    char* hostname;
    char* port;
    List* list;
    pthread_mutex_t* s_mutex;
};

// Define a function to initialize the thread parameters
struct threadParameters* create_sendThread(char* hostname, char* port, List* list, pthread_mutex_t* mutex);

// Free the allocated memory for the thread parameters
void freeParameters(struct threadParameters* par);

void* send_joinThread();