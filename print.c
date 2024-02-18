#include "print.h"

static List* outputList;
static pthread_mutex_t pMutex;
static pthread_t printThread;

void* print_process(){
    while (1) {
        pthread_mutex_lock(&pMutex);

        if (List_count(outputList) > 0) {
            char* outputMsg = List_first(outputList);

            // Output the message
            printf("Output: %s", outputMsg);

            List_remove(outputList);

            free(outputMsg); // Free the memory allocated for the message
        }

        pthread_mutex_unlock(&pMutex);
    }

    return NULL;
}

void* print_createThread(List* list, pthread_mutex_t mutex){
    outputList = list;
    pMutex = mutex;

    // Create the output thread with error check
    if (pthread_create(&printThread, NULL, print_process, NULL) != 0) {
        perror("keyboard_createOutputThread: pthread_create error");
        // Handle error, maybe exit or return
        return NULL;
    }
}


void* print_joinThread(){
    pthread_join(printThread, NULL);
}