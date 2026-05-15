#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "block.h"
#define BITS_PER_BYTE 8

// There are two blocks in the file system that represent free/not-free things:

// Block 1: inode map
// Block 2: Block map

// Finds the lowest clear (not-set, zero) bit in a byte
int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < BITS_PER_BYTE; i++)
        if (!(x & (1 << i)))
            return i;

    return -1;
}

// Set a specific bit to the value in set (0 or 1)
void set_free(unsigned char *block, int num, int set)
{
    int byte_num = num / BITS_PER_BYTE;
    int bit_num = num % BITS_PER_BYTE;
    int mask = 1 << bit_num;
    if (set)
    {
        block[byte_num] |= mask;
    }
    else
    {
        block[byte_num] &= ~mask;
    }
}

// Find a 0 bit and return its index (i.e. the block number that corresponds to this bit).
int find_free(unsigned char *block)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        int bit_index = find_low_clear_bit(block[i]);
        if (bit_index != -1)
        {
            return i * BITS_PER_BYTE + bit_index;
        }
    }
    return -1;
}
