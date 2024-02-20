#include "keyboard.h"
#include "threadcancel.h"

static List* inputList;
static pthread_mutex_t *keyMutex;
static pthread_t keyThread;
static pthread_cond_t* sendCondition;

#define MESSAGE_LENGTH 1024

void* keyboard_input(){

    while(1){
        char input[MESSAGE_LENGTH];
        //getting user response
        fgets(input,sizeof(input),stdin);

        //Otherwise storing user input in a list
        if(strcmp(input,"\n")!=0){
            //lock the mutex
            pthread_mutex_lock(keyMutex);
            {
                List_append(inputList,input);
                //Signal send to resume working 
                pthread_cond_signal(sendCondition);
            }
            pthread_mutex_unlock(keyMutex);
        }
        
        //Checking if the user wants to end the program
        if(strcmp(input,"!\n")==0){
            printf("You have ended the program!\n");
            cancelReceive();
            cancelPrint();
            cancelSend();
            cancelKeyboard();
            break;
        }
    }
}

void* keyboard_createThread(List* list1, pthread_mutex_t *mutex, pthread_cond_t *s_condition){
    inputList = list1;
    keyMutex = mutex;
    sendCondition = s_condition;
    pthread_create(&keyThread,NULL,keyboard_input,NULL);
    keyCancelInit(keyThread);
}

void* keyboard_joinThread(){
    pthread_join(keyThread,NULL);
}