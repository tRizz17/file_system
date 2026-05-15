#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "block.h"
#include "image.h"
#include "free.h"

unsigned char *bread(int block_num, unsigned char *block)
{
    lseek(image_fd, (block_num * BLOCK_SIZE), SEEK_SET);
    read(image_fd, block, BLOCK_SIZE);
    return block;
}

void bwrite(int block_num, unsigned char *block)
{
    lseek(image_fd, (block_num * BLOCK_SIZE), SEEK_SET);
    write(image_fd, block, BLOCK_SIZE);
}

int alloc(void)
{
    unsigned char free_data_block[BLOCK_SIZE];
    bread(FREE_DATA_BLOCK_NUM, free_data_block);
    int free_bit = find_free(free_data_block);
    if (free_bit == -1)
        return free_bit;
    set_free(free_data_block, free_bit, 1);
    bwrite(FREE_DATA_BLOCK_NUM, free_data_block);
    return free_bit;
}
