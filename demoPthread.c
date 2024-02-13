#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
void* slowPrintThread(void* Usermsg){
    char* msg = Usermsg;
    for( int i = 0; i< strlen(msg); i++){
        printf("%c",msg[i]);
        fflush(stdout);
        usleep(50000);
    }
}

int main(int argc, char**args){
    printf("***********DONE PROGRAM**********\n");
    //Creating a p_thread
    pthread_t threadPID;
    pthread_create(&threadPID, //PID by pointer
    NULL, //Attributes
    slowPrintThread,// Function
    "Hello welcome to this program!\n"); //Arguments for the function(slowPrintThread)
    

    pthread_t threadPID2;
    pthread_create(&threadPID2, //PID by pointer
    NULL, //Attributes
    slowPrintThread,// Function
    "***************************OPPS!\n"); //Arguments for the function(slowPrintThread)
   

    pthread_join(threadPID,NULL);
    pthread_join(threadPID2,NULL);
    print("Hello\n");

}
