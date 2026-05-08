#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int image_fd;

int image_open(char *filename, int truncate)
{
    int flags = O_RDWR | O_CREAT;
    if (truncate)
        flags |= O_TRUNC;
    image_fd = open(filename, flags, 0600);
    return image_fd;
}

int image_close(void)
{
    return close(image_fd);
}