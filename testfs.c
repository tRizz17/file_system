#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "image.h"
#include "block.h"
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

int main(void)
{
    CTEST_VERBOSE(1);
    test_image_open_and_close();
    test_write_read_new();
    test_find_only_available_bit();
    test_set_specific_bit();
    test_ialloc();
    test_alloc();
    CTEST_RESULTS();
    CTEST_EXIT();
}

#endif