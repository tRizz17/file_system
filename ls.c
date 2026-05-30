#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "dir.h"
#include "image.h"
#include "mkfs.h"

void ls(void)
{
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(0);

    while (directory_get(dir, &ent) != -1)
        printf("%d %s\n", ent.inode_num, ent.name);

    directory_close(dir);
}

int main(void)
{
    mkfs();
    image_open("file_system", 0);
    ls();
    image_close();
    return 0;
}