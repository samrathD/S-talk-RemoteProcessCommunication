#include "receiveProcess.h"

#define PORT 22110 // Replace
#define MSG_MAX_LEN 1024 // Replacable

static List* receiveList;
static pthread_mutex_t receiveMutex;

static pthread_t receiveThread;

static int recvSocket;

static int my_port;

pthread_cond_t recvCondition;

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
        pthread_mutex_lock(&receiveMutex);
        {
            List_append(receiveList,messageRx);
        }
        pthread_mutex_unlock(&receiveMutex);

        printf("message received: %s\n", messageRx);
    }
}

void* receive_createThread(List* list2, int port, int socket, pthread_mutex_t mutex, pthread_cond_t condition){
    receiveList = list2;
    receiveMutex = mutex;
    recvCondition = condition;
    //convert port from string to integer
    //assign port to myport
    my_port = port;
    recvSocket = socket;
    pthread_create(&receiveThread, NULL, receive_input, NULL); 
}

//Function to join the threads
void* receive_joinThread(){
    pthread_join(receiveThread,NULL);
}