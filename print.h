#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

void* print_process();
void* print_createThread(List* list, pthread_mutex_t mutex);
void* print_joinThread();