#include "cellgrid.h"
#include <iostream>
#include <fstream>
using namespace std;

// reads a file containing 1's and 0's and the dimesnsions to expect and
// creates a Cellgrid object
Cellgrid::Cellgrid(const char *fname, int m, int n) {
    // assign row and col counters
    rows = m;
    cols = n;

    // create the grid on the heap, this will have to be deallocated later in
    // the destructor
    grid = new bool[rows*cols];

    // create a file reader stream to read data from a file
    ifstream infile;

    // open a stream to the given file
    infile.open(fname);

    // we are using row-major order in order to store this grid in 1D
    // instead of accessing grid like grid[a][b], we instead do:
    // grid[(a * number of columns) + b]
    for(int i = 0; i < rows; i++) {
        // calculate "(a * number of columns)" from above
        // in order to save some unnecessary computation
        int base = i * cols;
        for(int offset = 0; offset < cols; offset++) {
            // read data from filestream and store it in grid
            infile >> grid[base + offset];
        }
    }

    // close the filestream once we are done with it
    infile.close();
}

// destruct object where not implicit to the program
Cellgrid::~Cellgrid() {
    // since we allocated memory on the heap, we must deallocate that memory
    delete[] grid;
}

// This is a private recursive helper function that implements the backtracking
// solution for problem 1
void Cellgrid::countCellNumbers(int row, int col, int conn, bool visited[], int& cellCount) {
    // Mark current cell as visited
    visited[row*cols + col] = true;
    cellCount++;

    // Check all adjacent cells that are connected
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            if(conn == 4 && abs(i) == abs(j)) continue;

            int newRow = row + i;
            int newCol = col + j;
            // Check if adjacent cell is within grid boundaries
            if(newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                int index = newRow*cols + newCol;
                // Check if adjacent cell is not visited and contains a 1
                if(!visited[index] && grid[index]) {
                    countCellNumbers(newRow, newCol, conn, visited, cellCount);
                }
            }
        }
    }
}

// this is the function call which will be made by the autograder to test
// your implementation for problem 1. (row, col) is the "start point" for
// counting the cells, conn is the type of connection to consider (4 or 8).
int Cellgrid::countCells(int row, int col, int conn) {
    bool visited[rows*cols] = {false};
    int cellCount = 0;
    countCellNumbers(row, col, conn, visited, cellCount);
    return cellCount;
}

int Cellgrid::countBlobs(int conn){
    //TODO:
    // implement solution for problem 2

    // this is where your private helper method which does the
    // backtracking should be called

    // this will prevent compiler error but should be changed
    // to return the blob count which you have found using
    // your helper function
    return 0;
}

// this function prints your cellgrid in 2D format without trailing
// whitespace. Use this function to aid in debugging if you wish
void Cellgrid::print() {
    for(int i = 0; i < rows; i++) {
        int base = i * cols;
        std::cout << grid[base];
        for(int offset = 1; offset < cols; offset++) {
            std::cout << ' ' << grid[base + offset];
        }
        std::cout << std::endl;
    }
}