Conway's Game of Life

A simple C++ implementation of Conway's Game of Life, a cellular automaton where cells evolve over multiple generations based on the number of neighboring cells that are alive.

Features
Accepts a customizable grid size.
Supports a specified number of generations.
Uses * to represent a living cell.
Uses . to represent a dead cell.
Updates the grid according to the rules of Conway's Game of Life.
Prints the final state of the grid.
Requirements
C++ compiler supporting C++11 or later.
How to Compile
g++ main.cpp -o gameoflife

How to Run
./gameoflife


The program expects input in the following format:

rows columns generations
grid


The grid should contain * and . characters separated by spaces.

Example Input
5 5 1
. . . . .
. . * . .
. . * . .
. . * . .
. . . . .


This creates a 5×5 grid and runs the simulation for 1 generation.

Output
. . . . .
. . . . .
. * * * .
. . . . .
. . . . .

How It Works

For every generation, the program examines each cell and counts its eight possible neighbors.

The rules are:

A living cell with fewer than 2 neighbors dies from underpopulation.
A living cell with 2 or 3 neighbors survives.
A living cell with more than 3 neighbors dies from overpopulation.
A dead cell with exactly 3 neighbors becomes alive.

The program creates a copy of the grid for each generation so that changes do not affect the calculation of other cells during the same generation.

Input

The first line contains three integers:

r c g


Where:

r = number of rows.
c = number of columns.
g = number of generations to simulate.

The following r lines contain the initial grid.

Output

After all generations have been simulated, the program prints the final grid to the console.

Note

This program simulates Conway's Game of Life but does not display each generation. Only the final state after the specified number of generations is printed.
