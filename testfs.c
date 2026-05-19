#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "image.h"
#include "block.h"
#include "inode.h"
#define CTEST_ENABLE
#include "ctest.h"
#include "free.h"

#ifdef CTEST_ENABLE

void test_image_open_and_close(void)
{
    CTEST_ASSERT(image_open("test", 1) != -1, "open with truncate");
    CTEST_ASSERT(image_fd >= 0, "global image_fd was set");
    CTEST_ASSERT(image_close() == 0, "close returns success");
}

void test_write_read_new(void)
{
    CTEST_ASSERT(image_open("test", 1) != -1, "open with truncate");
    CTEST_ASSERT(image_fd >= 0, "global image_fd was set");
    unsigned char out[BLOCK_SIZE];
    unsigned char in[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++)
        out[i] = i % 256;

    bwrite(3, out);

    CTEST_ASSERT(bread(3, in) == in, "bread returns correct pointer");
    CTEST_ASSERT(memcmp(out, in, BLOCK_SIZE) == 0, "data is stored correctly");
    CTEST_ASSERT(image_close() == 0, "close returns success");
}

void test_find_only_available_bit(void)
{
    unsigned char test_block[BLOCK_SIZE];
    memset(test_block, 0xFF, BLOCK_SIZE);
    set_free(test_block, 25, 0);
    CTEST_ASSERT(find_free(test_block) == 25, "find_free finds the only free bit in the block");
}

void test_set_specific_bit(void)
{
    int num = 25;
    unsigned char test_block[BLOCK_SIZE] = {0};
    set_free(test_block, num, 1);
    CTEST_ASSERT(test_block[3] == 0x02, "set_free correctly sets specific bit to 1");
}

void test_ialloc(void)
{
    image_open("ialloc_test", 1);
    unsigned char test_block[BLOCK_SIZE] = {0};
    bwrite(INODE_BLOCK_NUM, test_block);
    int test_count = 200;
    int last;
    for (int i = 0; i < test_count; i++)
    {
        last = ialloc();
    }
    CTEST_ASSERT(last == (test_count - 1), "ialloc finds next free bit");
    image_close();
}

void test_alloc(void)
{
    image_open("alloc_test", 1);
    unsigned char test_block[BLOCK_SIZE] = {0};
    bwrite(FREE_DATA_BLOCK_NUM, test_block);
    int test_count = 200;
    int last;
    for (int i = 0; i < test_count; i++)
    {
        last = alloc();
    }
    CTEST_ASSERT(last == (test_count - 1), "alloc finds next free bit");
    image_close();
}

void test_ialloc_overflow(void)
{
    image_open("ialloc_test", 1);
    unsigned char test_block[BLOCK_SIZE] = {0};
    bwrite(INODE_BLOCK_NUM, test_block);
    int last;
    for (int i = 0; i < (BLOCK_SIZE * 8) + 1; i++)
    {
        last = ialloc();
    }

    CTEST_ASSERT(last == -1, "ialloc returns -1 if inode map full");
    image_close();
}

void test_alloc_overflow(void)
{
    image_open("alloc_test", 1);
    unsigned char test_block[BLOCK_SIZE] = {0};
    bwrite(FREE_DATA_BLOCK_NUM, test_block);
    int last;
    for (int i = 0; i < (BLOCK_SIZE * 8) + 1; i++)
    {
        last = alloc();
    }
    CTEST_ASSERT(last == -1, "alloc returns -1 if data map full");
    image_close();
}

void test_ialloc_incore_methods(void)
{
    int test_count = 10;
    struct inode *base = incore_find_free();
    for (int i = 0; i < test_count; i++)
    {
        struct inode test = {
            .size = 1024,
            .owner_id = 1,
            .permissions = 255,
            .flags = 0,
            .link_count = 1,
            .block_ptr = {0},
            .ref_count = 1,
            .inode_num = i,
        };
        struct inode *free_spot = incore_find_free();
        *free_spot = test;
    }
    struct inode *last = incore_find_free();
    CTEST_ASSERT(last - base == 10, "incore_find_free finds the next free incore inode");
    CTEST_ASSERT(incore_find(5) == base + 5, "incore_find finds the correct inode");
    incore_free_all();
    struct inode *new_base = incore_find_free();
    CTEST_ASSERT(new_base == base, "incore free all successfully frees all structs in incore");
}

void test_ialloc_incore_methods_overflow(void)
{
    int test_count = 64;
    struct inode *base = incore_find_free();
    for (int i = 0; i < test_count; i++)
    {
        struct inode test = {
            .size = 1024,
            .owner_id = 1,
            .permissions = 255,
            .flags = 0,
            .link_count = 1,
            .block_ptr = {0},
            .ref_count = 1,
            .inode_num = i,
        };
        struct inode *free_spot = incore_find_free();
        *free_spot = test;
    }
    struct inode *last = incore_find_free();
    CTEST_ASSERT(last == NULL, "incore_find_free returns NULL if incore full");
    incore_free_all();
    CTEST_ASSERT(incore_find(5) == NULL, "incore_find returns NULL if struct ref_count == 0");
    struct inode *new_base = incore_find_free();
    CTEST_ASSERT(new_base == base, "incore free all successfully frees all structs in incore");
}

void test_read_and_write_inode_owner_id(void)
{
    int test_inode_num = 5;
    int test_owner_id = 17;

    struct inode write_test = {
        .size = 1024,
        .owner_id = test_owner_id,
        .permissions = 255,
        .flags = 0,
        .link_count = 1,
        .block_ptr = {0},
        .ref_count = 0,
        .inode_num = test_inode_num,
    };
    write_inode(&write_test);

    struct inode read_test;
    read_inode(&read_test, test_inode_num);
    CTEST_ASSERT(read_test.owner_id == test_owner_id, "write_inode successfully writes, read_inode successfully reads owner_id field");
}

void test_read_and_write_inode_flags(void)
{
    int test_inode_num = 8;
    int test_owner_id = 17;
    int test_flags = 20;

    struct inode write_test = {
        .size = 1024,
        .owner_id = test_owner_id,
        .permissions = 255,
        .flags = test_flags,
        .link_count = 1,
        .block_ptr = {0},
        .ref_count = 0,
        .inode_num = test_inode_num,
    };
    write_inode(&write_test);

    struct inode read_test;
    read_inode(&read_test, test_inode_num);
    CTEST_ASSERT(read_test.flags == test_flags, "write_inode successfully writes, read_inode successfully reads flags field");
}

int main(void)
{
    CTEST_VERBOSE(1);
    test_image_open_and_close();
    test_write_read_new();
    test_find_only_available_bit();
    test_set_specific_bit();
    test_ialloc();
    test_alloc();
    test_ialloc_overflow();
    test_alloc_overflow();
    test_ialloc_incore_methods();
    test_ialloc_incore_methods_overflow();
    test_read_and_write_inode_owner_id();
    test_read_and_write_inode_flags();
    CTEST_RESULTS();
    CTEST_EXIT();
}

#endif