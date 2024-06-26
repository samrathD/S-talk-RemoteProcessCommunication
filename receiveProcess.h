#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "list.h"

void* receive_input(void* unused);

void* receive_createThread(List* list2, int socket, pthread_mutex_t *mutex, pthread_cond_t *condition);

void* receive_joinThread();