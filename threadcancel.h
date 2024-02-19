#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "keyboard.h"
#include "print.h"
#include "receiveProcess.h"
#include "sendProcess.h"
#include "list.h"

void cancelReceive();
void cancelKeyboard();
void cancelSend();
void cancelPrint();
