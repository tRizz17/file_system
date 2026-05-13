#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


void mkfs(void);


// Write an empty (all zeros) block to the following:

// Block 0: Superblock
// Block 1: inode map
// Block 3: inode data block 0
// Block 4: inode data block 1
// Block 5: inode data block 2
// Block 6: inode data block 3
// Then mark the first 7 blocks as non-free in the block map (block 2).

// You can do this by calling alloc() repeatedly, or (and this is a far more elegant solution), 
// figure out exactly which byte values need to go in the block map to mark the first 7 blocks as non-free and bwrite() them directly.
// Hint: 0x7f.