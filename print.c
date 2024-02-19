#include "print.h"
#include "threadcancel.h"

static List* outputList;
static pthread_mutex_t *pMutex;
static pthread_t printThread;
static pthread_cond_t *printCondition;

void* print_process(){
    while (1) {
        pthread_mutex_lock(pMutex);
        pthread_cond_wait(printCondition,pMutex);
        List_first(outputList);
        char* outputMsg = List_remove(outputList);
        if(strcmp(outputMsg,"!\n")==0){
            exit(-1);
        }
        // Output the message
        printf("THEM: %s", outputMsg);
        pthread_mutex_unlock(pMutex);
    }
}

void* print_createThread(List* list, pthread_mutex_t *mutex, pthread_cond_t *condition){
    outputList = list;
    pMutex = mutex;
    printCondition = condition;
    // Create the output thread with error check
    if (pthread_create(&printThread, NULL, print_process, NULL) != 0) {
        perror("keyboard_createOutputThread: pthread_create error");
        // Handle error, maybe exit or return
        return NULL;
    }
    printCancelInit(printThread);
}


void* print_joinThread(){
    pthread_join(printThread, NULL);
}