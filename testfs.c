#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <image.h>
#include <block.h>
#define CTEST_ENABLE
#include <ctest.h>

int main(void)
{

    image_open("test.txt", 1);

    image_close();
}