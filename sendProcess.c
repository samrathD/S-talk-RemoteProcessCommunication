#include "sendProcess.h"

struct addrinfo *serverinfo;

struct threadParameters* create_sendThread(char* hostname, char* port, List* list, pthread_mutex_t mutex) {
    struct threadParameters* par = (struct threadParameters*)malloc(sizeof(struct threadParameters));
    par->hostname = hostname;
    par->port = port;
    par->list = list;
    par->s_mutex = mutex;
    return par;
}

// Free the allocated memory for the thread parameters
void freeParameters(struct threadParameters* par) {
    free(par);
}

void* send_process(void* arg) {
    struct threadParameters* par = (struct threadParameters*)arg;

    struct addrinfo info, *p;
    int getAddr;
    int numbytes;

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_DGRAM;

    getAddr = getaddrinfo(par->hostname, par->port, &info, &p);

    // Create and bind socket
    int socketID = -1;
    for (struct addrinfo* temp = p; temp != NULL; temp = temp->ai_next) {
        socketID = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);

        // Check for socket creation errors here

        bind(socketID, temp->ai_addr, temp->ai_addrlen);
    }

    // Loop for sending
    while (1) {
        int count = 0;
        do {
            count++;
            char* message;
            pthread_mutex_lock(&par->s_mutex);
            {
                message = List_trim(par->list);
            }
            pthread_mutex_unlock(&par->s_mutex);

            // send
            numbytes = sendto(socketID, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);

            if (!strcmp(message, "!\n") && count == 1) {
                free(message);
                message = NULL;
                return NULL;
            }

            // De-allocating message
            free(message);
            message = NULL;

        } while (List_count(par->list) != 0);
    }

    // close the socket here if needed
    return NULL;
}

void* send_joinThread(pthread_t senderThread) {
    pthread_join(senderThread, NULL);
}