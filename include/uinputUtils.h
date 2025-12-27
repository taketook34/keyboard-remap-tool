#ifndef UINPUTUTILS_H
#define UINPUTUTILS_H

#include <stdio.h>

int setupReadEvent();
void closeReadEvent(int real_keyboard_fd);

int setupUinput();
void closeUinput(int virtual_keyboard_fd);

void emit(int fd, int type, int code, int val);

#endif