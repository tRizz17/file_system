#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "free.h"
#include "block.h"
#include "inode.h"

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
