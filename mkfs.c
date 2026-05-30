#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "image.h"
#include "block.h"
#include "inode.h"
#include "pack.h"
#define RESERVED_BLOCKS_NUM 7
#define ROOT_SIZE 64
#define DIR_FLAG 2
#define DIR_ENTRY_SIZE 32

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
    struct inode *root_inode = ialloc();
    int root_block_num = alloc();

    root_inode->flags = DIR_FLAG;
    root_inode->size = ROOT_SIZE;
    root_inode->block_ptr[0] = root_block_num;

    unsigned char block[BLOCK_SIZE] = {0};
    unsigned char *pos = block;
    char *name = ".";

    for (int i = 0; i < 2; i++)
    {
        write_u16(pos, root_inode->inode_num);
        strcpy((char *)pos + 2, name);
        pos = block + DIR_ENTRY_SIZE;
        name = "..";
    }
    bwrite(root_block_num, block);
    iput(root_inode);

    image_close();
}