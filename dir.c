#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "inode.h"
#include "dir.h"

struct directory *directory_open(int inode_num)
{

    struct inode *inode = iget(inode_num);
    if (inode == NULL)
        return NULL;

    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = inode;
    dir->offset = 0;
    return dir;
}

void directory_close(struct directory *d)
{
    iput(d->inode);
    free(d);
}
