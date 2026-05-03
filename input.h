#ifndef INPUT_H
#define INPUT_H

/**
 * Common interface struct to save keyboard input data
 */
typedef struct KeyboardEvent
{
    char key[16];
    int keyValue;

} KeyboardEvent_t;

int input_open(const char *device_path);
int input_read(int fd, KeyboardEvent_t *event);
int input_close(int fd);

#endif