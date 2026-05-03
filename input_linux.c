#if defined(__linux__)

#include "input.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "keyboardMapping.h"
#include <linux/input.h>
#include <errno.h>

int input_open(const char device_path[])
{
    return open(device_path, O_RDONLY);
}

int input_read(int fd, KeyboardEvent_t *event)
{
    struct input_event ie;

    if (read(fd, &ie, sizeof(ie)) == sizeof(ie))
    {
        if (ie.type != EV_KEY)
            return -1;

        static int shift;
        if (ie.code == 42 || ie.code == 54)
        {
            shift = (ie.value == KEY_PRESSED || ie.value == KEY_RELEASED);
            return 0;
        }

        if (!(ie.value == KEY_PRESSED || ie.value == KEY_REPEAT))
            return -1;
        if (ie.code > KEYMAP_SIZE || keymap[ie.code] == NULL)
            return -1;

        if (shift && keymap[ie.code][1] == '\0')
        {
            snprintf(event->key, sizeof(event->key), "%c", keymap[ie.code] - 32);
        }
        else
        {
            snprintf(event->key, sizeof(event->key), "%c", keymap[ie.code]);
        }

        event->keyValue = ie.value;
        return 1;
    }

    return -1;
}

int input_close(int fd)
{
    close(fd);
}

#endif