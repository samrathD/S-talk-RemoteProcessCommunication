#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "keyboard.h"
#include "print.h"
#include "receiveProcess.h"
#include "sendProcess.h"
#include "list.h"

void keyCancelInit(pthread_t thread);
void printCancelInit(pthread_t thread);
void receiveCancelInit(pthread_t thread);
void sendCancelInit(pthread_t thread);
void cancelReceive();
void cancelKeyboard();
void cancelSend();
void cancelPrint();