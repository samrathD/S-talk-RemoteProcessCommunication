#include "sendProcess.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo *serverinfo;

static List* sendList;
static pthread_mutex_t sendMutex;

static pthread_t sendThread;

static int sendSocket;

static char* theirPort;

static char*hostname;


// Free the allocated memory for the thread parameters
// void freeParameters(struct threadParameters* par) {
//     free(par);
// }

void* send_input(void* arg) {
    // struct threadParameters* par = (struct threadParameters*)arg;

    // struct addrinfo info, *p;
    struct addrinfo info,*p;
    int getAddr;
    int numbytes;

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_DGRAM;

    getAddr = getaddrinfo(hostname, theirPort, &info, &p);

    // Create and bind socket
    int socketID = -1;
    for (struct addrinfo* temp = p; temp != NULL; temp = temp->ai_next) {
        socketID = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);

        // Check for socket creation errors here

        // You might want to bind the socket here as well
        bind(socketID, temp->ai_addr, temp->ai_addrlen);
    }
    char* message;
    // Loop for sending
    while (1) {
        pthread_mutex_lock(&sendMutex);
        {
            List_first(sendList);
            message = List_remove(sendList);
        }
        pthread_mutex_unlock(&sendMutex);

        int count = 0;
        numbytes = sendto(socketID, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);
        if (!strcmp(message, "!\n") && count == 1) {
                free(message);
                message = NULL;
                return NULL;
            }
        free(message);
        message = NULL;
    }

    //     do {
    //         count++;
    //         char* message;
    //         pthread_mutex_lock(&par->s_mutex);
    //         {
    //             message = List_trim(par->list);
    //         }
    //         pthread_mutex_unlock(&par->s_mutex);

    //         // send
    //         numbytes = sendto(socketID, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);

    //         if (!strcmp(message, "!\n") && count == 1) {
    //             free(message);
    //             message = NULL;
    //             return NULL;
    //         }

    //         // De-allocating message
    //         free(message);
    //         message = NULL;

    //     } while (List_count(par->list) != 0);
    // }

    // close the socket here if needed

    return NULL;
}

void * send_createThread(char* host, int port, List* list2, pthread_mutex_t mutex){
    sendList = list2;
    sendMutex = mutex;

    sprintf(theirPort,"%d",port);
    // theirPort = port;
    hostname = host;

    pthread_create(&sendThread,NULL,send_input,NULL);


}

void* send_joinThread() {
    pthread_join(sendThread, NULL);
}