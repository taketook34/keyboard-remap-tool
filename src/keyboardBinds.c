#include <keyboardBinds.h>

#include <stdio.h>
#include <string.h>

void parseLine(char line[MAX_LINE]) {
    char *word = strtok(line, " \t");
    while (word) {
        printf("  Слово: %s\n", word);
        word = strtok(NULL, " \t");
    }
}

void returnCode(struct input_event *ev) {
    if (ev->code == KEY_W) {
        ev->code = KEY_Q;
    }
}
