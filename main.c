#include "Socket.h"
#include "keyboard.h"
#include"receiveProcess.h"
#include "sendProcess.h"
//First create a function that creates a socket

//Create a function that that takes the keyboard argument - Keyboard

//Create a function that send the data over to the other socket - Send

//Create a function that prints the data on the screen


 
//*******************
int main(){
//Create 2 lists 
//      - List 1 stores the argument from keyboard and send it over to the other socket
//      - List 2 stores recieves the data and prints it on the screen

// Create 2 mutex 
//       - One for list 1
//       - One for list 2

//Initialize sockets
//       - One for list 1
//       - One for list 2


//Keyboard 
//      - put this in a pthread
//      - busy wait while loop always waiting for user response
//      - gets the user response and strncpy response into a malloc string
//      - append it in the list-1 

//

//Send
//      - put this in a pthread
//      - Arguments host's name and port
//      - busy waiting to send a response(if the list is empty then pthread_wait())
//      - if not empty remove message form list1 and send over the network to the server
//      - Deallocate message

//Recieve
//      - put this in a pthread
//      - Arguments recieving port and the list2
//      - Busy wait while loop constantly waiting to recieve the message
//      - Add the message in list 2 

//Print_screen
//      - put this in a pthread
//      - Arguments list2
//      - Busy wait while loop constantly waiting to print output
//      - If list 2 is empty pthread_wait()
//      - Remove message from the list and print to the screen


//Shutdown and free list
//      - shutdown all the threads and sockets
//      - free the lists


    List*list1 = List_create();
    List*list2 = List_create();
    pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;

    printf("Starting program.....\n Press '!' to quit \n Enter a message - \n");

    //Creating a pthread for keyboard input
    keyboard_createThread(list1,mutex_1); 
    receive_createThread(list2,"22110",mutex_2);
    send_createThread(list1,"127.0.0.1","22110",mutex_1);

    //Joining the threads  
    keyboard_joinThread();
    receive_joinThread();
    send_joinThread();
}