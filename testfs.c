#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "image.h"
#include "block.h"
#define CTEST_ENABLE
#include "ctest.h"


int main(void)
{

    image_open("test", 1);
    
    unsigned char written[BLOCK_SIZE];
    unsigned char read_back[BLOCK_SIZE] = {0};
    for (int i = 0; i < BLOCK_SIZE; i++) written[i] = i % 256;

    bwrite(3, written);

    bread(3, read_back);

    image_close();
}