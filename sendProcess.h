#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

void* send_process(char* hostname, char* port, List* list, pthread_mutex_t* mutex);