#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "free.h"
#include "block.h"
#include "inode.h"
#include "pack.h"
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3
#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)


int ialloc(void)
{
    unsigned char inode_block[BLOCK_SIZE];
    bread(INODE_BLOCK_NUM, inode_block);
    int free_inode_num = find_free(inode_block);
    if (free_inode_num == -1)
        return free_inode_num;
    set_free(inode_block, free_inode_num, 1);
    bwrite(INODE_BLOCK_NUM, inode_block);
    int block_num = free_inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = free_inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;
    return free_inode_num;
}
