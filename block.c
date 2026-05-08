#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <image.h>

const int BLOCK_SIZE = 4096;

unsigned char *bread(int block_num, unsigned char *block)
{
    l_seek(image_fd, (block_num * BLOCK_SIZE), SEEK_SET);
    read(image_fd, block, BLOCK_SIZE);
    return block;
}

void bwrite(int block_num, unsigned char *block)
{
    l_seek(image_fd, (block_num * BLOCK_SIZE), SEEK_SET);
    write(image_fd, block, strlen(block));
    return block;
}
