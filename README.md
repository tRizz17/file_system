# Very very simple file system


## Building

Command line:

* `make` to build. An executable called `testfs` will be produced.
* `make test` to build and run tests.
* `make clean` to clean up all build products except the executable.
* `make pristine` to clean up all build products entirely.

## Files

* `image.c`: Functions for opening and closing a file
* `block.c`: Functions for reading and writing to blocks
* `ctest.h`: Macros for writing tests
* `testfs.c`: Runs tests

## Data

Planned block layout:

* Block 0: Superblock
* Block 1: Inode map
* Block 2: Block map
* Blocks 3-6: Inode data blocks
* Blocks 7-end: File data blocks

## Functions

* `main()`
  * `test_image_open_and_close()`
    * `image_open()`
    * `image_close()`
  * `test_write_read_new()`
    * `image_open()`
    * `bwrite()`
    * `bread()`
    * `image_close()`