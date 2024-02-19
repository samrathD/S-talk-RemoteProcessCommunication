#include "keyboard.h"
#include"receiveProcess.h"
#include "sendProcess.h"
#include "print.h"
//First create a function that creates a socket -

//Create a function that that takes the keyboard argument - Keyboard

//Create a function that send the data over to the other socket - Send

//Create a function that prints the data on the screen - Print

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

//*******************
int main(int argc, char**args){
    if(argc!=4){
        printf("%d\n",argc);
        printf("%s\n",args[0]);
        printf("Please provide local port number, remote IP and remote port number\n");
        exit(EXIT_FAILURE);
    }

    int myport = atoi(args[1]);
    char *remoteIP = args[2];
    char* remotePort = args[3];

    // int myport = 22111;
    // char*remoteIP = "asb9838nu-e08";
    // char* remotePort = "22111";
    //Creating lists
    List*list1 = List_create();
    List*list2 = List_create();
    pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t sendCondition = PTHREAD_COND_INITIALIZER;
    pthread_cond_t printCondition = PTHREAD_COND_INITIALIZER;

    printf("Starting program.....\n Press '!' to quit \n Enter a message - \n");

    //Storing the information of the port that I am talking to 
    struct sockaddr_in sockAddr;
    memset(&sockAddr,0,sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(myport);

    struct addrinfo *recvInfo;
    
    //Create a socket to be used by both the sender and the receiver
    int socketDescriptor = socket(AF_INET,SOCK_DGRAM,0);

    if(socketDescriptor == -1){
        printf("OOPS! Cannot create a socket!\n");
    }

    //Bind the socket
    if(bind(socketDescriptor,(struct sockaddr*)&sockAddr,sizeof(sockAddr))!=0){
        printf("Cannot bind the socket");
        close(socketDescriptor);
        return -1;
    }

    //Creating a pthread for keyboard input
    keyboard_createThread(list1,&mutex_1,&sendCondition);
    receive_createThread(list2,socketDescriptor,&mutex_2, &printCondition);
    send_createThread(remoteIP,remotePort, socketDescriptor,list1,&mutex_1,&sendCondition);
    print_createThread(list2,&mutex_2,&printCondition);

    //Joining the threads 
    keyboard_joinThread();
    receive_joinThread();
    send_joinThread();
    print_joinThread();

    close(socketDescriptor);
    pthread_mutex_destroy(&mutex_1);
    pthread_mutex_destroy(&mutex_2);
    List_free(list1, NULL);
    List_free(list2, NULL);
}