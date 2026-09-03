Sudoku Solver

A simple C++ program that checks whether a given 9×9 Sudoku solution is valid.

Features
Reads a Sudoku grid from standard input.
Checks that every row contains numbers from 1–9 without duplicates.
Checks that every 3×3 box contains numbers from 1–9 without duplicates.
Prints whether the Sudoku solution is valid or incorrect.
Requirements
C++ compiler supporting C++11 or later.
How to Compile
g++ main.cpp -o sudoku

How to Run

Provide the 81 Sudoku numbers as input:

./sudoku < input.txt


You can also enter the numbers manually:

./sudoku


Enter all 81 numbers, separated by spaces or newlines.

Example Input
5 3 4 6 7 8 9 1 2
6 7 2 1 9 5 3 4 8
1 9 8 3 4 2 5 6 7
8 5 9 7 6 1 4 2 3
4 2 6 8 5 3 7 9 1
7 1 3 9 2 4 8 5 6
9 6 1 5 3 7 2 8 4
2 8 7 4 1 9 6 3 5
3 4 5 2 8 6 1 7 9

Output

For a valid Sudoku:

Solution is good!


For an invalid Sudoku:

Wrong solution!

How It Works

The program performs two main checks:

Rows: Makes sure each row contains unique values between 1 and 9.
3×3 Boxes: Makes sure each of the nine 3×3 sections contains unique values between 1 and 9.

If any invalid value or duplicate is found, the program immediately reports Wrong solution!.

Note

Despite the name, this program does not solve an incomplete Sudoku puzzle. It validates a completed 9×9 Sudoku grid.
