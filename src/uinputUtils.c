#include <uinputUtils.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <string.h>



int setupReadEvent() {
    int real_keyboard_fd = open("/dev/input/event4", O_RDONLY | O_NONBLOCK);
    if (real_keyboard_fd < 0) { perror("open"); return -1; }


    if (ioctl(real_keyboard_fd, EVIOCGRAB, 1) < 0) {
        perror("EVIOCGRAB");
        return -1;
    }


    return real_keyboard_fd;
}

void closeReadEvent(int real_keyboard_fd) {
    ioctl(real_keyboard_fd, EVIOCGRAB, 0);
    close(real_keyboard_fd);
}

int setupUinput() {
    
    int virtual_keyboard_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (virtual_keyboard_fd < 0) {
        perror("open uinput");
        return -1;
    }

    struct uinput_setup usetup;

    ioctl(virtual_keyboard_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(virtual_keyboard_fd, UI_SET_EVBIT, EV_SYN);
    for (int i = 0; i < 256; i++)
        ioctl(virtual_keyboard_fd, UI_SET_KEYBIT, i);
    // ioctl(virtual_keyboard_fd, UI_SET_KEYBIT, KEY_SPACE);
    // ioctl(virtual_keyboard_fd, UI_SET_KEYBIT, KEY_A);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234; /* sample vendor */
    usetup.id.product = 0x5678; /* sample product */
    strcpy(usetup.name, "Example device");

    ioctl(virtual_keyboard_fd, UI_DEV_SETUP, &usetup);
    ioctl(virtual_keyboard_fd, UI_DEV_CREATE);
    return virtual_keyboard_fd;
}

void closeUinput(int virtual_keyboard_fd) {
    ioctl(virtual_keyboard_fd, UI_DEV_DESTROY);
    close(virtual_keyboard_fd);
}

void emit(int fd, int type, int code, int val) {
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));
}