#include "keyboard.h"

static List* inputList;
static pthread_mutex_t *keyMutex;
static pthread_t keyThread;
static pthread_cond_t* sendCondition;

void* keyboard_input(){

    while(1){
        char input[1024];
        //getting user response
        fgets(input,sizeof(input),stdin);

        //Checking if the user wants to end the program
        if(strcmp(input,"!\n")==0){
            printf("You have ended the program!\n");
            pthread_cancel(keyThread);
            exit(-1);
        }

        //Otherwise storing user input in a list
        if(strcmp(input,"\n")!=0){
            //Allocate memory for the string
            char* newMsg = malloc(sizeof(input)*sizeof(char) + 1); // null terminator
            if(newMsg == NULL){
                exit(-1);
                
            }
            strcpy(newMsg,input);
            //lock the mutex
            pthread_mutex_lock(keyMutex);
            {
                List_append(inputList,newMsg);
                //Signal send to resume working 
                pthread_cond_signal(sendCondition);

              //printf("The message is - %s", newMsg);
            }
            pthread_mutex_unlock(keyMutex);
        }
    }
}

void* keyboard_createThread(List* list1, pthread_mutex_t *mutex, pthread_cond_t *condition){
    inputList = list1;
    keyMutex = mutex;
    sendCondition = condition;
    pthread_create(&keyThread,NULL,keyboard_input,NULL);
}

void* keyboard_joinThread(){
    pthread_join(keyThread,NULL);
}