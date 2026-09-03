8-Puzzle Checker

A simple C++ program that simulates moves on a 3×3 sliding puzzle, commonly known as the 8-puzzle, and checks whether the final board is in the correct solved order.

Features
Uses a 3×3 board to represent the puzzle.
Uses 0 to represent the empty space.
Accepts movement commands to move the empty space.
Supports four movement directions:
U — Up
D — Down
L — Left
R — Right
Checks whether the final board is sorted into the solved configuration.
Prints whether the solution is correct or incorrect.
Requirements
C++ compiler supporting C++11 or later.
How to Compile
g++ main.cpp -o puzzle

How to Run
./puzzle


The program first expects 9 integers representing the starting puzzle configuration, followed by movement commands.

Example Input
1 2 3
4 5 6
7 0 8
R


The 0 represents the empty space. The R command moves the empty space to the right, resulting in the solved puzzle.

Output

For a correctly solved puzzle:

Solution is good!


For an incorrectly solved puzzle:

Wrong solution!

How It Works

The program performs the following steps:

Reads the initial 3×3 puzzle board.
Finds the location of the empty space (0).
Reads movement commands until there is no more input.
Moves the empty space in the requested direction if the move is valid.
Converts the final board into a single list of numbers.
Checks whether the numbers are in ascending order and the empty space is not in the first position.
Prints the result.
Movement Commands
Command	Direction
U	Move empty space up
D	Move empty space down
L	Move empty space left
R	Move empty space right

If a move would take the empty space outside the board, that move is simply ignored.

Solved Configuration

The expected solved board is:

1 2 3
4 5 6
7 8 0


The program determines this by checking that the values are in ascending order and that 0 is at the end.

Note

This program does not find a solution automatically. It takes a starting board and a sequence of moves, applies those moves, and then checks whether the resulting board is solved.
