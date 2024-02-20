#include "keyboard.h"
#include "receiveProcess.h"
#include "sendProcess.h"
#include "print.h"
#include "threadcancel.h"

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

    //Creating lists
    List*list1 = List_create();
    List*list2 = List_create();
    pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t sendCondition = PTHREAD_COND_INITIALIZER;
    pthread_cond_t printCondition = PTHREAD_COND_INITIALIZER;

    printf("Starting program.....\n Press '!' to quit \n Enter your message - \n");

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
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    int numbytes;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(remoteIP, remotePort, &hints, &serverInfo) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    //Creating a pthread for keyboard input
    keyboard_createThread(list1,&mutex_1,&sendCondition);
    receive_createThread(list2,socketDescriptor,&mutex_2, &printCondition);
    send_createThread(remoteIP,remotePort, socketDescriptor,list1,&mutex_1,&sendCondition,serverInfo);
    print_createThread(list2,&mutex_2,&printCondition);

    //Joining the threads 
    keyboard_joinThread();
    receive_joinThread(); 
    send_joinThread();
    print_joinThread();

    // clean up
    close(socketDescriptor);
    freeaddrinfo(serverInfo);
    pthread_mutex_destroy(&mutex_1);
    pthread_mutex_destroy(&mutex_2);
    pthread_cond_destroy(&sendCondition);
    pthread_cond_destroy(&printCondition);
    List_free(list1, NULL);
    List_free(list2, NULL); 
    printf("\n**********Program ENDED****************\n");
    return 0;
}