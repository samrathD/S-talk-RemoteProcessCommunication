#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#define MSG_MAX_LEN 1024
#define PORT 22110

int main(int argCount, char** args){
    printf("Samrath's Net Listen Test on UDP port %d:\n", PORT);
    printf("Connect using: \n");
    printf("    netcat -u 127.0.0.1 %d\n", PORT);


    struct sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    //Sotring the file descriptor
    //File descriptor is a unique integer that the OS uses to open a file
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    bind(socketDescriptor,(struct sockaddr*)&sin, sizeof(sin));
    char messageRx[MSG_MAX_LEN];
    //Recieving the data
    while(1 && strcmp(messageRx,"!\n")!=0){
        // struct sockaddr_in sinRemote;
        // unsigned int sin_len = sizeof(sinRemote);
        // // char messageRx[MSG_MAX_LEN];
        
        // int bytesRx = recvfrom(socketDescriptor,messageRx,
        // MSG_MAX_LEN,0,(struct sockaddr*)&sinRemote, &sin_len);

        // //Null terminated(string)
        // // int terminatedIdx = (bytesRx< MSG_MAX_LEN)? bytesRx:MSG_MAX_LEN - 1;

        // // messageRx[terminatedIdx] = 0;
        // printf("Message Recieved(%d bytes): '%s'",bytesRx,messageRx);

        // //Extract the value form the messsage:
        // int incMe = atoi(messageRx);

        // //Create Reply
        // char messageTx[MSG_MAX_LEN];
        // sprintf(messageTx,"Math: %d + 1 = %d\n", incMe, incMe+1);

        // //Send Reply
        // sin_len = sizeof(sinRemote);
        // sendto(socketDescriptor,messageTx,strlen(messageTx),0,(struct sockaddr*)&sinRemote,sin_len);
        
        List* receiveList = List_create();
        // static pthread_mutex_t receiveMutex = INITIALIZE;

        static int my_port;

        //pthread_cond_t *printCondition;
        
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];
        memset(messageRx, 0, sizeof(messageRx));
        

        int bytesRx = recvfrom(socketDescriptor, messageRx, MSG_MAX_LEN, 0, 
                                (struct sockaddr*) &sinRemote, &sin_len);
        printf("Hello\n");                        
        //Null terminated(string)
        int terminatedIdx = (bytesRx< MSG_MAX_LEN)? bytesRx:MSG_MAX_LEN - 1;
        messageRx[terminatedIdx] = 0;
       
        //Lock Mutex
       // pthread_mutex_lock(receiveMutex);
        {
            printf("Message recieved\n");
            List_append(receiveList,messageRx);
           // pthread_cond_signal(printCondition);
        }
       // pthread_mutex_unlock(receiveMutex);

        printf("message received: %s\n", messageRx);

    }
   // close(socketDescriptor);
}
