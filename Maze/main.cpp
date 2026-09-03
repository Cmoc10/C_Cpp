#include "maze.h"

void check_neighbors(std::vector<std::pair<int,int>>& neighbors, int i, int j, int rmax, int cmax, std::vector<std::vector<Block>>& cells, std::vector<char>& neighbor_sides){
    std::pair<int, int> temp_copy;
    if(i != 0 && cells[i-1][j].get_visited() == false){
        temp_copy.first = i-1;
        temp_copy.second = j;
        neighbors.push_back(temp_copy);
        neighbor_sides.push_back('N');
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
    if(j != 0 && cells[i][j-1].get_visited() == false){
        temp_copy.first = i;
        temp_copy.second = j-1;
        neighbors.push_back(temp_copy);
        neighbor_sides.push_back('W');
    }
    return;
};

char get_side(char initial_side){
    switch (initial_side)
    {
    case 'N':
        return 'S';
    case 'W':
        return 'E';
    case 'E':
        return 'W';
    case 'S':
        return 'N';
    
    default:
        return initial_side;
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
    std::vector<std::pair<int, int>> location_storage;
    cells[0][0].setWall_without_visit('N');
    cells[rows-1][columns-1].setWall_without_visit('S');
    std::pair<int, int> location;
    location.first = 0;
    location.second = 0;
    location_storage.push_back(location);
    int i=0;
    int j=0;
    std::vector<std::pair<int,int>> neighbors(4);
    std::vector<char> neighbor_sides(4);
    while(location_storage.size() != 0){
        location = location_storage.back();
        location_storage.pop_back();
        i = location.first;
        j = location.second;
        neighbors.clear();
        neighbor_sides.clear();
        check_neighbors(neighbors, i, j, rows, columns, cells, neighbor_sides);
        if(neighbors.size() != 0){
            location_storage.push_back(location);
            int idx = std::rand() / ((RAND_MAX + 1u) / neighbors.size());
            char side = neighbor_sides[idx];
            char otherside = get_side(side);
            cells[neighbors[idx].first][neighbors[idx].second].setWall(otherside);
            cells[i][j].setWall(side);
            cell_move(i,j, side);
            location.first = i;
            location.second = j;
            location_storage.push_back(location);
        }
    }
    return;
};

int main(int argc, char** argv){
if(argc != 5){
    std::cout << "WRONG" << "\n";
    return 0;
}
int seed = std::stoi(argv[1]);
int rows = std::stoi(argv[2]);
int cols = std::stoi(argv[3]);
std::string fname = argv[4];
std::srand(seed);
std::vector<std::vector<Block>> cells(rows, std::vector<Block>(cols));
Block temp;
for(int i=0; i<rows; i++){
    for(int j = 0; j<cols; j++){
        cells[i].push_back(temp);
    }
}
maze_maker(cells, rows, cols);
std::ofstream myfile;
myfile.open(fname);
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            myfile << cells[i][j].get_walls() << " ";
        }
        myfile << "\n";
    }
myfile.close();
return 0;
}