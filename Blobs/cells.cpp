#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct cell{
    int value;
    int blob = 0;
    bool visited = false;
};

void check_neighbors_4(std::vector<std::vector<cell>>& grid, int row, int col, int cur_blob, int row_max, int col_max, int& count);
void check_neighbors_8(std::vector<std::vector<cell>>& grid, int row, int col, int cur_blob, int row_max, int col_max, int& count);

int check_neighbors(std::vector<std::vector<cell>>& grid, int row, int col, int cur_blob, int row_max, int col_max, int conn_type){
    int count = 0;
    if(grid[row-1][col-1].value == 0){
        return count;
    }
    else if(conn_type == 4){
        count = 1;
        check_neighbors_4(grid, row-1, col-1, cur_blob+1, row_max, col_max, count);
    }
    else{
        count = 1;
        check_neighbors_8(grid, row-1, col-1, cur_blob+1, row_max, col_max, count);
    }
    return count;
}

void check_neighbors_4(std::vector<std::vector<cell>>& grid, int row, int col, int cur_blob, int row_max, int col_max, int& count){
    grid[row][col].visited = true;
    if(row+1 != row_max){
        if(grid[row+1][col].value == 1 && !grid[row+1][col].visited){
        count++;
        check_neighbors_4(grid,row+1, col, cur_blob, row_max, col_max, count);
        }
    }
    if(row-1 >= 0){ 
        if(grid[row-1][col].value == 1 && !grid[row-1][col].visited){
            count++;
            check_neighbors_4(grid,row-1, col, cur_blob, row_max, col_max, count);
        }
    }
    if(col+1 != col_max){
        if(grid[row][col+1].value == 1 && !grid[row][col+1].visited){
        count++;
        check_neighbors_4(grid,row, col+1, cur_blob, row_max, col_max, count);
        }
    }
    if(col-1 >= 0){
        if(grid[row][col-1].value == 1 && !grid[row][col-1].visited){
            count++;
            check_neighbors_4(grid,row, col-1, cur_blob, row_max, col_max, count);
        }
    }
    return;
}

void check_neighbors_8(std::vector<std::vector<cell>>& grid, int row, int col, int cur_blob, int row_max, int col_max, int& count){
    grid[row][col].visited = true;
    if(row+1 != row_max && grid[row+1][col].value == 1 && !grid[row+1][col].visited){
        count++;
        check_neighbors_8(grid,row+1, col, cur_blob, row_max, col_max, count);
    }
    if(row-1 >= 0 && grid[row-1][col].value == 1 && !grid[row-1][col].visited){
        count++;
        check_neighbors_8(grid,row-1, col, cur_blob, row_max, col_max, count);
    }
    if(col+1 != col_max && grid[row][col+1].value == 1 && !grid[row][col+1].visited){
        count++;
        check_neighbors_8(grid,row, col+1, cur_blob, row_max, col_max, count);
    }
    if(col-1 >= 0 && grid[row][col-1].value == 1 && !grid[row][col-1].visited){
        count++;
        check_neighbors_8(grid,row, col-1, cur_blob, row_max, col_max, count);
    }
    //checking diagonals
    if(row+1 != row_max && col+1 != col_max && grid[row+1][col+1].value == 1 && !grid[row+1][col+1].visited){
        count++;
        check_neighbors_8(grid,row+1, col+1, cur_blob, row_max, col_max, count);
    }
    if(row+1 != row_max && col > 0 && grid[row+1][col-1].value == 1 && !grid[row+1][col-1].visited){
        count++;
        check_neighbors_8(grid,row+1, col-1, cur_blob, row_max, col_max, count);
    }
    if(col+1 != col_max && row-1 >= 0 && grid[row-1][col+1].value == 1 && !grid[row-1][col+1].visited){
        count++;
        check_neighbors_8(grid,row-1, col+1, cur_blob, row_max, col_max, count);
    }
    if(col-1 >= 0 && row-1 >= 0 && grid[row-1][col-1].value == 1 && !grid[row-1][col-1].visited){
        count++;
        check_neighbors_8(grid,row-1, col-1, cur_blob, row_max, col_max, count);
    }
    return;
}

int main(int argc, char** argv){
    std::string fname = argv[1];
    int col = atoi(argv[3]);
    int row = atoi(argv[2]);
    int start_row = atoi(argv[4]);
    int start_col = atoi(argv[5]);
    int conn_type = atoi(argv[6]);
    std::string line;
    std::vector<int> file_data;
	std::ifstream file(fname);
    cell temp;
    std::vector<std::vector<cell>> matrix(row, std::vector<cell> (col,temp));

    // Read the first line from the file & store into vector
    while(std::getline(file, line)){
        std::stringstream ss(line);

        int temp_int;
        while(ss >> temp_int){
            file_data.push_back(temp_int);
        }
    }
    int count;
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            matrix[i][j].value = file_data[count];
            count++;
        }
    }
    std::cout << check_neighbors(matrix, start_row, start_col, 0, row, col, conn_type) << "\n";
    return 0;
}