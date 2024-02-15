#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

//keyboard_input takes two arguments
//Pointer to the list and pointer to the mutex
void* keyboard_input();
void* keyboard_createThread(List* list1, pthread_mutex_t mutex);
void* keyboard_joinThread();