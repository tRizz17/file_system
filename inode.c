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
#define OWNER_ID_OFFSET 4
#define PERMISSIONS_OFFSET 6
#define FLAGS_OFFSET 7
#define LINK_COUNT_OFFSET 8
#define BLOCK_PTR_ARRAY_OFFSET 9
#define BLOCK_PTR_OFFSET 2

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

struct inode *ialloc(void)
{
    unsigned char inode_block[BLOCK_SIZE];
    bread(INODE_BLOCK_NUM, inode_block);
    int free_inode_num = find_free(inode_block);
    if (free_inode_num == -1)
        return NULL;
    struct inode *free_inode = iget(free_inode_num);
    if (!free_inode)
        return NULL;
    set_free(inode_block, free_inode_num, 1);
    bwrite(INODE_BLOCK_NUM, inode_block);
    free_inode->size = 0;
    free_inode->owner_id = 0;
    free_inode->permissions = 0;
    free_inode->flags = 0;
    for (int i = 0; i < INODE_PTR_COUNT; i++)
        free_inode->block_ptr[i] = 0;
    free_inode->inode_num = free_inode_num;
    write_inode(free_inode);
    return free_inode;
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
    in->owner_id = read_u16(struct_block + block_offset_bytes + OWNER_ID_OFFSET);
    in->permissions = read_u8(struct_block + block_offset_bytes + PERMISSIONS_OFFSET);
    in->flags = read_u8(struct_block + block_offset_bytes + FLAGS_OFFSET);
    in->link_count = read_u8(struct_block + block_offset_bytes + LINK_COUNT_OFFSET);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        in->block_ptr[i] = read_u16(struct_block + block_offset_bytes + BLOCK_PTR_ARRAY_OFFSET + i * BLOCK_PTR_OFFSET);
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
    write_u16(struct_block + block_offset_bytes + OWNER_ID_OFFSET, in->owner_id);
    write_u8(struct_block + block_offset_bytes + PERMISSIONS_OFFSET, in->permissions);
    write_u8(struct_block + block_offset_bytes + FLAGS_OFFSET, in->flags);
    write_u8(struct_block + block_offset_bytes + LINK_COUNT_OFFSET, in->link_count);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        write_u16(struct_block + block_offset_bytes + BLOCK_PTR_ARRAY_OFFSET + i * BLOCK_PTR_OFFSET, in->block_ptr[i]);
    }

    bwrite(block_num, struct_block);
}

struct inode *iget(int inode_num)
{
    struct inode *free_inode = incore_find(inode_num);
    if (free_inode != NULL)
    {
        free_inode->ref_count += 1;
        return free_inode;
    }
    // If free_inode NULL
    free_inode = incore_find_free();
    if (free_inode == NULL)
    {
        return NULL;
    }
    read_inode(free_inode, inode_num);
    free_inode->ref_count = 1;
    free_inode->inode_num = inode_num;
    return free_inode;
}

void iput(struct inode *in)
{
    if (in->ref_count == 0)
        return;
    in->ref_count -= 1;
    if (in->ref_count == 0)
        write_inode(in);
}
