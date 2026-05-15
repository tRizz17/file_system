#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "image.h"
#include "block.h"
#define RESERVED_BLOCKS_NUM 7

void mkfs(void)
{
    image_open("file_system", 1);
    for (int i = 0; i < RESERVED_BLOCKS_NUM; i++)
    {
        unsigned char block[BLOCK_SIZE] = {0};
        if (i == 2)
            block[0] = 0x7f;
        bwrite(i, block);
    }
    image_close();
}