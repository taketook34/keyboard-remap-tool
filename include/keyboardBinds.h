#ifndef KEYBOARDBINDS_H
#define KEYBOARDBINDS_H

#include <linux/input.h>
#define MAX_LINE 100

struct KeyBind {
    int oldkey;
    int newkey;
};

void returnCode(struct input_event *ev);

#endif