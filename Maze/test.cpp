#include "maze.h"

void check_neighbors(std::vector<std::pair<int,int>>& neighbors, int i, int j, int rmax, int cmax, std::vector<std::vector<Block>> cells, std::vector<char>& neighbor_sides){
    std::pair<int, int> temp_copy;
    if(i != 0 && cells[i-1][j].get_visited() == false){
        temp_copy.first = i-1;
        temp_copy.second = j;
        neighbors.push_back(temp_copy);
        neighbor_sides.push_back('N');
    }
    if(j != 0 && cells[i][j-1].get_visited() == false){
        temp_copy.first = i;
        temp_copy.second = j-1;
        neighbors.push_back(temp_copy);
        neighbor_sides.push_back('W');
    }
    if(i != rmax-1 && cells[i+1][j].get_visited() == false){
        temp_copy.first = i+1;
        temp_copy.second = j;
        neighbors.push_back(temp_copy);
        neighbor_sides.push_back('S');
    }
    if(j != cmax-1 && cells[i][j+1].get_visited() == false){
        temp_copy.first = i;
        temp_copy.second = j+1;
        neighbors.push_back(temp_copy);
        neighbor_sides.push_back('E');
    }
    return;
};

char get_side(char initial_side){
    switch (initial_side)
    {
    case 'N':
        return 'S';
        break;
    case 'W':
        return 'E';
        break;
    case 'E':
        return 'W';
        break;
    case 'S':
        return 'N';
        break;
    
    default:
        return initial_side;
        break;
    }
}

void cell_move(int& i, int& j, char side){
    switch (side)
    {
    case 'N':
        i -=1;
        break;
    case 'W':
        j -= 1;
        break;
    case 'E':
        j += 1;
        break;
    case 'S':
        i += 1;
        break;
    
    default:

        break;
    }
}

void maze_maker(std::vector<std::vector<Block>>& cells, int rows, int columns){
    return;
};

int main(){
    int rows = 10;
    int cols = 10;
    std::srand(0);
    std::vector<std::vector<Block>> cells(rows, std::vector<Block>(cols));
    Block temp;
    for(int i=0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            cells[i].push_back(temp);
        }
    }
    std::vector<std::pair<int, int>> location_storage;
    cells[0][0].setWall('N');
    cells[rows-1][cols-1].setWall('S');
    std::pair<int, int> location;
    location.first = 0;
    location.second = 0;
    location_storage.push_back(location);
    int i=0;
    int j=0;
    while(location_storage.size() != 0){
        location = location_storage.back();
        location_storage.pop_back();
        i = location.first;
        j = location.second;
        std::vector<std::pair<int,int>> neighbors = {};
        std::vector<char> neighbor_sides = {};
        check_neighbors(neighbors, i, j, rows, cols, cells, neighbor_sides);
        if(neighbors.size() != 0){
            location_storage.push_back(location);
            int idx = std::rand() / ((RAND_MAX + 1u) / neighbors.size());
            char side = neighbor_sides[idx];
            char otherside = get_side(side);
            cells[neighbors[idx].first][neighbors[idx].second].setWall(side);
            cells[i][j].setWall(otherside);
            cell_move(i,j, side);
            location.first = i;
            location.second = j;
            location_storage.push_back(location);
        }
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            std::cout << cells[i][j].get_walls() << " ";
        }
        std::cout << "\n";
    }
}