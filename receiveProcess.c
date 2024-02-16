#include "receiveProcess.h"

#define PORT 22110 // Replace
#define MSG_MAX_LEN 1024 // Replacable

static int socketDescriptor = -1;
static List* receiveList;
static pthread_mutex_t receiveMutex;

static pthread_t receiveThread;

int my_port;

//Pass port numbers
void* receive_input(void* unused) {
    // address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    // create udp socket
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
    while (1) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];

        memset(messageRx, 0, sizeof(messageRx));
        int bytesRx = recvfrom(socketDescriptor, messageRx, MSG_MAX_LEN, 0, 
                                (struct sockaddr*) &sinRemote, &sin_len);
        //Null terminated(string)
        int terminatedIdx = (bytesRx< MSG_MAX_LEN)? bytesRx:MSG_MAX_LEN - 1;
        messageRx[terminatedIdx] = 0;
        // char* inputMessage = malloc(sizeof(char)*(bytesRx+1));
        // strcpy(inputMessage,messageRx);
        
        //Lock Mutex
        pthread_mutex_lock(&receiveMutex);
        {
            List_append(receiveList,messageRx);
        }
        pthread_mutex_unlock(&receiveMutex);
        printf("message received: %s\n", messageRx);

    }
}

void* receive_createThread(List* list2, char* port, pthread_mutex_t mutex){
    receiveList = list2;

    receiveMutex = mutex;

    //convert port from string to integer
    //assign port to myport
    my_port = atoi(port);

    pthread_create(&receiveThread, NULL, receive_input, NULL); 

}

//Function to join the threads
void* receive_joinThread(){
    pthread_join(receiveThread,NULL);
}