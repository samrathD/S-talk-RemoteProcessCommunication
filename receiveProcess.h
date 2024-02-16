#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "list.h"

void* receiveThread(void* unused);

void* receive_createThread(List* list2, char* port, pthread_mutex_t mutex);

void* receive_joinThread();