#include "receiveProcess.h"
#include "threadcancel.h"


#define MSG_MAX_LEN 1024 // Replacable

static List* receiveList;
static pthread_mutex_t *receiveMutex;

static pthread_t receiveThread;

static int recvSocket;

pthread_cond_t *printCondition; 

//Pass port numbers
void* receive_input(void* unused) {
    
    while (1) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];
        memset(messageRx, 0, sizeof(messageRx));
        

        int bytesRx = recvfrom(recvSocket, messageRx, MSG_MAX_LEN, 0, 
                                (struct sockaddr*) &sinRemote, &sin_len);
        //Null terminated(string)
        int terminatedIdx = (bytesRx< MSG_MAX_LEN)? bytesRx:MSG_MAX_LEN - 1;
        messageRx[terminatedIdx] = 0;
       
        //Lock Mutex
        pthread_mutex_lock(receiveMutex);
        {
            List_append(receiveList,strdup(messageRx));
            pthread_cond_signal(printCondition);
        }
        pthread_mutex_unlock(receiveMutex);

        if (strcmp(messageRx, "!\n") == 0){
            fputs("\nThey ended the chat \n", stdout);

            cancelReceive();
            cancelPrint();
            cancelKeyboard();
            cancelSend();
            break;;
        }
    }
}

void* receive_createThread(List* list2, int socket, pthread_mutex_t* mutex, pthread_cond_t *condition){
    receiveList = list2;
    receiveMutex = mutex;
    printCondition = condition;
    recvSocket = socket;
    pthread_create(&receiveThread, NULL, receive_input, NULL); 
    receiveCancelInit(receiveThread);
}

//Function to join the threads
void* receive_joinThread(){
    pthread_join(receiveThread,NULL);
}