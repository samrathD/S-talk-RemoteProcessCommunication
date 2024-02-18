#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

void* print_process();
void* print_createThread(List* list, pthread_mutex_t *mutex, pthread_cond_t *condition);
void* send_signal();
void* print_joinThread();