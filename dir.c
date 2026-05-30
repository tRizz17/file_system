#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct directory
{
    struct inode *inode;
    unsigned int offset;
};

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
