#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


void set_free(unsigned char *block, int num, int set);


int find_free(unsigned char *block);

int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;
    
    return -1;
}
