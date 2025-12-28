#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "uinputUtils.h"
#include "keyboardBinds.h"

static bool running = true;

void handle_exit(int sig) {
    //(void)sig;
    puts("keyboard-remap-tool - stop working");
    running = false;
}

int main() {
    
    /* BEGIN Opening files */
    int real_keyboard_fd = setupReadEvent();
    int virtual_keyboard_fd = setupUinput();
    if (virtual_keyboard_fd == -1 || real_keyboard_fd == -1) {
        return 1;
    }
    /* END opening files */
    /* BEGIN defining additioonals structs */
    struct input_event ev;
    /* END defining additioonals structs */


    /* BEGIN Set up signal mask for program */
    struct sigaction sa;
    sa.sa_handler = handle_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    /* END Set up signal mask for program */


    /* BEGIN Read data from config file */
    FILE *file = fopen("file.conf", "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    readConfFile(file);
    fclose(file);
    /* END Read data from config file */

    sleep(1);

    /* BEGIN Read data from real keyboard */
    while (running) {
        if (read(real_keyboard_fd, &ev, sizeof(ev)) != sizeof(ev)) {
            continue;
        }

        if (ev.type == EV_KEY) {
            // if (ev.code == KEY_W) {
            //     ev.code = KEY_Q;
            // }
            //printf("%d\n", ev.code);
            returnCode(&ev);
            //printf("%d\n===\n", ev.code);

            write(virtual_keyboard_fd, &ev, sizeof(ev));
            emit(virtual_keyboard_fd, EV_SYN, SYN_REPORT, 0);
        }
    }
    /* END Read data from real keyboard */

    sleep(1);

    /* BEGIN Closing fds and finish program correctly */
    closeReadEvent(real_keyboard_fd);
    closeUinput(virtual_keyboard_fd);
    return 0;
    /* END Closing fds and finish program correctly */
}
