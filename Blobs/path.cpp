#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct point{
    int height = 0;
    bool visited = false;
};

void find_points_low(std::vector<std::vector<point>> matrix, std::pair<int,int>& location){
    int low_val = matrix[0][0].height;
    int low_row = 0;
    int low_col = 0;
    for(int i=0; i<matrix.size(); i++){
        for(int j=0; j<matrix[i].size(); j++){
            if(matrix[i][j].height < low_val){
                low_val = matrix[i][j].height;
                low_row = i;
                low_col = j;
            }
        }
    }
    location.first = low_row;
    location.second = low_col;
    return;
}

void find_points_high(std::vector<std::vector<point>> matrix, std::pair<int,int>& location){
    int high_val = matrix[0][0].height;
    int high_row = matrix.size();
    int high_col = matrix[0].size();
    for(int i=0; i<matrix.size(); i++){
        for(int j=0; j<matrix[i].size(); j++){
            if(matrix[i][j].height > high_val){
                high_val = matrix[i][j].height;
                high_row = i;
                high_col = j;
            }
        }
    }
    location.first = high_row;
    location.second = high_col;
    return;
}

bool check_move(int val, int row, int col, int row_max, int col_max, std::vector<std::vector<point>>& matrix, int target_row, int target_col){
    //if two nums next to each other are the exact same no worko
    if(target_row == row && target_col == col){
        return true;
    }
    matrix[row][col].visited = true;
    if(row + 1 < row_max){
        if(val <= matrix[row+1][col].height && !matrix[row+1][col].visited){
            if(check_move(matrix[row+1][col].height, row+1, col, row_max, col_max, matrix, target_row, target_col)){
                return true;
            }
        }
    }
    if(row - 1 >= 0){
        if(val <= matrix[row-1][col].height && !matrix[row-1][col].visited){
            if(check_move(matrix[row-1][col].height, row-1, col, row_max, col_max, matrix, target_row, target_col)){
                return true;
            }
        }
    }
    if(col + 1 < col_max){
        if(val <= matrix[row][col+1].height && !matrix[row][col+1].visited){
            if(check_move(matrix[row][col+1].height, row, col+1, row_max, col_max, matrix, target_row, target_col)){
                return true;
            }
        }
    }
    if(col - 1 >= 0){
        if(val <= matrix[row][col-1].height && !matrix[row][col-1].visited){
            if(check_move(matrix[row][col-1].height, row, col-1, row_max, col_max, matrix, target_row, target_col)){
                return true;
            }
        }
    }
    return false;
}

bool find_path(std::pair<int, int> low, std::pair<int, int> high, std::vector<std::vector<point>>& matrix){
    return check_move(matrix[low.first][low.second].height, low.first, low.second, matrix.size(), matrix[0].size(), matrix, high.first, high.second);
}

int main(int argc, char** argv){
    std::string fname = argv[1];
    int col = atoi(argv[3]);
    int row = atoi(argv[2]);
    std::string line;
    std::vector<int> file_data;
	std::ifstream file(fname);
    point temp;
    std::vector<std::vector<point>> matrix(row, std::vector<point> (col,temp));

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
            matrix[i][j].height = file_data[count];
            count++;
        }
    }
    std::pair<int, int> low;
    std::pair<int, int> high;
    bool is_possible;
    find_points_low(matrix, low);
    find_points_high(matrix, high);
    std::cout << low.first + 1 << " " << low.second+1 << " " << high.first+1 << " " << high.second + 1 << " ";
    is_possible = find_path(low,high, matrix);
    if(is_possible == true){
        std::cout << "yes\n";
    }
    else{
        std::cout << "no\n";
    }
    return 0;
}