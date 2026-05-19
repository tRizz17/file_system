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
    unsigned char struct_block[BLOCK_SIZE];

    bread(block_num, struct_block);

    in->size = read_u32(struct_block + block_offset_bytes);
    in->owner_id = read_u16(struct_block + block_offset_bytes + 4);
    in->permissions = read_u8(struct_block + block_offset_bytes + 6);
    in->flags = read_u8(struct_block + block_offset_bytes + 7);
    in->link_count = read_u8(struct_block + block_offset_bytes + 8);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        in->block_ptr[i] = read_u16(struct_block + block_offset_bytes + 9 + i * 2);
    }
}

void write_inode(struct inode *in)
{
    int inode_num = in->inode_num;
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;
    unsigned char struct_block[BLOCK_SIZE];

    bread(block_num, struct_block);

    write_u32(struct_block + block_offset_bytes, in->size);
    write_u16(struct_block + block_offset_bytes + 4, in->owner_id);
    write_u8(struct_block + block_offset_bytes + 6, in->permissions);
    write_u8(struct_block + block_offset_bytes + 7, in->flags);
    write_u8(struct_block + block_offset_bytes + 9, in->link_count);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        write_u16(struct_block + block_offset_bytes + 9 + i * 2, in->block_ptr[i]);
    }

    bwrite(block_num, struct_block);
}
