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
    int free_inode = find_free(inode_block);
    if (free_inode == -1)
        return free_inode;
    set_free(inode_block, free_inode, 1);
    bwrite(INODE_BLOCK_NUM, inode_block);
    return free_inode;
}
