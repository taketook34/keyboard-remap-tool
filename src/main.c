#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <getopt.h>

#include "uinputUtils.h"
#include "keyboardBinds.h"

static volatile bool running = true;

void handle_exit(int sig) {
    //(void)sig;
    puts("keyboard-remap-tool - stop working");
    running = false;
}

void print_usage(char *prog_name) {
    fprintf(stderr, "Usage: %s [-l] <device file> [configuration file]\nIf configuration file is not added, /etc/krt.conf will be used", prog_name);
}

int main(int argc, char** argv) {
    int opt;
    int l_flag = 0;

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
            case 'l':
                print_usage(argv[0]);;
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (optind >= argc) {
        print_usage(argv[0]);
        return 1;
    }
    char *device_file = argv[optind];
    char *configuration_file = "/etc/krt.conf";
    if (optind + 1 < argc) {
        configuration_file = argv[optind + 1];
    }
    /* BEGIN Opening files */
    int real_keyboard_fd = setupReadEvent(device_file);
    int virtual_keyboard_fd = setupUinput();
    if (virtual_keyboard_fd == -1 || real_keyboard_fd == -1) {
        return 1;
    }
    /* END opening files */

    /* BEGIN Set up signal mask for program */
    struct sigaction sa;
    sa.sa_handler = handle_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    /* END Set up signal mask for program */


    /* BEGIN Read data from config file */
    struct KeyBind binds_array[MAX_LINE];
    size_t barray_index = 0;
    FILE *file = fopen(configuration_file, "r");
    if (!file) { perror("fopen"); return 1; }
    readConfFile(file, binds_array, &barray_index);
    fclose(file);
    // printf("Uploaded configurations\n");
    // for (int i = 0; i < barray_index; i++) {
    //     printf(" %d instead of %d\n", binds_array[i].newkey, binds_array[i].oldkey);
    // }
    /* END Read data from config file */

    sleep(1);

    /* BEGIN Read data from real keyboard */
    struct input_event ev;
    while (running) {
        if (read(real_keyboard_fd, &ev, sizeof(ev)) != sizeof(ev)) {
            continue;
        }

        if (ev.type == EV_KEY) {
            //printf("%d\n", ev.code);
            returnCode(&ev, binds_array, barray_index);
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
