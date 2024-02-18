#include "sendProcess.h"

static pthread_t sendThread;

void* send_process(void* arg) {
    struct threadParameters* par = (struct threadParameters*)arg;
    
    List* list = par->list; // list
    pthread_mutex_t mutex = par->mutex; // mutex
    int UDPsocket = par->socket; // in main we make socket from port and address
    struct addrinfo *serverInfo = par->serverInfo; // 

    // Loop for sending
    while (1) {
        pthread_mutex_lock(&mutex);

        if (List_count(list)> 0){
            char* data = List_first(list);

            int sendDest = sendto(UDPsocket, data, strlen(data), 0, serverInfo->ai_addr, serverInfo->ai_addrlen);

            if (sendDest == -1){
                exit(-1);
            }   
            if (strcmp(data, "!\n") == 0){
                pthread_mutex_unlock(&mutex);
                break;
            }  

            List_remove(list);   
        }
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* send_createThread(List* list, char* destIP, char* destPort, pthread_mutex_t mutex) {
    
    // sending sockets
    struct addrinfo hints, *serverInfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_DGRAM; // udp
    getaddrinfo(destIP, destPort, &hints, &serverInfo);
    int UDPsocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    
    struct threadParameters par;
    par.list = list;
    par.mutex = mutex;
    par.socket = UDPsocket;
    par.serverInfo = serverInfo;

    // Create the send thread with error check
    if (pthread_create(&sendThread, NULL, send_process, (void*)&par) != 0) {
        perror("send_createThread: pthread_create error");
        // Handle error, maybe exit or return
        return NULL;
    }
}

void* send_joinThread() {
    pthread_join(sendThread, NULL);
}