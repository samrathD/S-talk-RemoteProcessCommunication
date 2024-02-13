#include "keyboard.h"

void* keyboard_input(List*list1, pthread_mutex_t mutex){

    while(1){
        char input[1024];
        //getting user response
        fgets(input,sizeof(input),stdin);

        //Checking if the user wants to end the program
        if(strcmp(input,"!\n")==0){
            printf("You have ended the program!\n");
            return;
        }

        //Otherwise storing user input in a list
        if(strcmp(input,"\n")!=0){
            //Allocate memory for the string
            char* newMsg = malloc(sizeof(input)*sizeof(char));
            if(newMsg == NULL){
                return -1;
            }
            newMsg = input;
            //lock the mutext
            pthread_mutex_lock(&mutex);
            {
                List_append(list1,newMsg);
                printf("The message is - %s", newMsg);
            }
            pthread_mutex_unlock(&mutex);
        }
    }
}