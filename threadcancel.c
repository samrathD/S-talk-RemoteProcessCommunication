#include "threadcancel.h"

static pthread_t keyboardThread, printThread, receiveThread, sendThread;

void cancelKeyboard(){
    pthread_cancel(keyboardThread);
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