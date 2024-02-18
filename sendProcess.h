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


void* send_input(void* arg);
// Define a function to initialize the thread parameters
void * send_createThread(char* host, int port, List* list2, pthread_mutex_t mutex,pthread_cond_t condition);

// Free the allocated memory for the thread parameters
//void* freeParameters(struct threadParameters* par);

void* send_joinThread();