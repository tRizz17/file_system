# Very very simple file system


## Building

Command line:

* `make` to build. An executable called `testfs` will be produced.
* `make test` to build and test.
* `make clean` to clean up all build products except the executable.
* `make pristine` to clean up all build products entirely.

## Files

* `image.c`: Functions for opening and closing a file
* `block.c`: Functions for reading and writing to blocks
* `ctest.h`: Macros for writing tests
* `testfs.c`: Runs tests

## Data

_[Description of the main data used in the program. Just list the major
data structures—not every variable.]_

There is an 80x24 2D array of Booleans that represents the cells, in
row-major order. Additionally, there's a second parallel array used to
double-buffer during the next generation computation.

If an array element is True, the cell is alive, otherwise it's dead.

## Functions

_[This is a tree of functions and their short descriptions]_

* `main()`
  * `init()`
    * `life_init()`: Initializes the cell array to random 
      * `grid_alloc()`: Allocate space for the cell grid
      * `randomize()`: Randomize the contents of the cell grid
    * `display_init()`: Initializes the display
      * `clear_screen()`
  * `run()`: Main game running routine
    * `life_update()`: Update cell data
      * `update_cell()`: updates a cell based on its neighbors
        * `get_neighbor_count()`: counts neighbors for a cell
    * `life_get_cells()`: Get cell status
    * `display_update()`: Display cells
      * `home_cursor()`
    * `delay()`: Delay between frames
  * `life_shutdown()`: Call before destruction (currently unreachable)
    * `grid_free()`: Free cell grid memory

## Notes

_[Any additional notes, bugs, etc.]_

* `^C` to quit.
* `life_init()` should allow different kinds of initializations, not
  just random.
* UI could be added so the user could draw patterns.
* Maybe add some preprogrammed patterns, spaceships, etc.
* Bug: display totally messed up on screens smaller than 80x24