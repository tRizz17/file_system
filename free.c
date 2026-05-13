#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// There are two blocks in the file system that represent free/not-free things:

// Block 1: inode map
// Block 2: Block map

// Set a specific bit to the value in set (0 or 1)
void set_free(unsigned char *block, int num, int set)
{
    int byte_num = num / 8;
    int bit_num = num % 8;
    int mask = 1 << bit_num;
    if (set)
        block[byte_num] |= mask;
    else
        block[byte_num] &= ~mask;
}

// Find a 0 bit and return its index (i.e. the block number that corresponds to this bit).
int find_free(unsigned char *block)
{
    
}

// Finds the lowest clear (not-set, zero) bit in a byte
int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;

    return -1;
}
