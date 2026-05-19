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

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

int ialloc(void)
{
    unsigned char inode_block[BLOCK_SIZE];
    bread(INODE_BLOCK_NUM, inode_block);
    int free_inode_num = find_free(inode_block);
    if (free_inode_num == -1)
        return free_inode_num;
    set_free(inode_block, free_inode_num, 1);
    bwrite(INODE_BLOCK_NUM, inode_block);

    return free_inode_num; // Keeping this for now so everything else works
}

struct inode *incore_find_free(void)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        if (incore[i].ref_count == 0)
        {
            return (incore + i);
        }
    }
    return NULL;
}

struct inode *incore_find(unsigned int inode_num)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        if (incore[i].ref_count != 0 && (incore[i].inode_num == inode_num))
        {
            return (incore + i);
        }
    }
    return NULL;
}

void incore_free_all(void)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        incore[i].ref_count = 0;
    }
}

void read_inode(struct inode *in, int inode_num)
{

    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    // For reference
    // struct inode
    // {
    //     unsigned int size;
    //     unsigned short owner_id;
    //     unsigned char permissions;
    //     unsigned char flags;
    //     unsigned char link_count;
    //     unsigned short block_ptr[INODE_PTR_COUNT];

    //     unsigned int ref_count; // in-core only
    //     unsigned int inode_num; // in-core only
    // };

    // Below is just example usage that will probably come in handy later
    // Assuming `block` is the array we read with `bread()`
    // int flags = read_u8(block_num + block_offset_bytes + 7);

    unsigned char struct_block[BLOCK_SIZE];
    bread(block_num, struct_block);
    in->size = read_u32(struct_block + block_offset_bytes);
    in->owner_id = read_u16(struct_block + block_offset_bytes + 2);
    in->permissions = read_u8(struct_block + block_offset_bytes + 2 + 1);
    in->flags = read_u8(struct_block + block_offset_bytes + 2 + 1 + 1);
    in->link_count = read_u8(struct_block + block_offset_bytes + 2 + 1 + 1 + 1);
    in->block_ptr[INODE_PTR_COUNT] = read_u16(struct_block + block_offset_bytes + 2 + 1 + 1 + 1 + 2);
}

void write_inode(struct inode *in)
{
    
}
