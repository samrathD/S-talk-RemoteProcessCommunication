#include "threadcancel.h"

static pthread_t keyThread, printThread, receiveThread, sendThread;

struct addrinfo* serverInfo;

void keyCancelInit(pthread_t thread){
    keyThread = thread;
}
void printCancelInit(pthread_t thread){
    printThread = thread;
}
void receiveCancelInit(pthread_t thread){
    receiveThread = thread;
}
void sendCancelInit(pthread_t thread){
    sendThread = thread;
}

void cancelKeyboard(){
    pthread_cancel(keyThread);
}
void cancelPrint(){
    pthread_cancel(printThread);
}
void cancelSend(){
    pthread_cancel(sendThread);
}
void cancelReceive(){
    pthread_cancel(receiveThread);
}
