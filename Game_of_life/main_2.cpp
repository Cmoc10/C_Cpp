#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

int main(){
    int r, c, g;
    std::cin >> r >> c >> g;
    std::vector<std::vector<std::string>> map(r, std::vector<std::string>(c));
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            std::string temp;
            std::cin >> temp;
            map[i][j] = temp;
        }
    }
    for(int n=0; n<g; n++){
        std::vector<std::vector<std::string>> copyMap(r, std::vector<std::string>(c));
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                int neighbors = 0;
                //Checking neighbors below
                if(i != r-1 && j != 0){
                    if(map[i+1][j-1] == "*"){
                        neighbors++;
                    }
                }
                if(i != r-1){
                    if(map[i+1][j] == "*"){
                        neighbors++;
                    }
                }
                if(i != r-1 && j != c-1){
                    if(map[i+1][j+1] == "*"){
                        neighbors++;
                    }
                }
                //Checking left and right neighbors
                if(j != c-1){
                    if(map[i][j+1] == "*"){
                        neighbors++;
                    }
                }
                if(j != 0){
                    if(map[i][j-1] == "*"){
                        neighbors++;
                    }
                }
                //Checking neighbors above
                if(i != 0){
                    if(map[i-1][j] == "*"){
                        neighbors++;
                    }
                }
                if(i != 0 && j != 0){
                    if(map[i-1][j-1] == "*"){
                        neighbors++;
                    }
                }
                if(i != 0 && j != c-1){
                    if(map[i-1][j+1] == "*"){
                        neighbors++;
                    }
                }
                //checking neighbors
                if(neighbors < 2){
                    copyMap[i][j] = ".";
                }
                else if(neighbors == 3){
                    copyMap[i][j] = "*";
                }
                else if(neighbors > 3){
                    copyMap[i][j] = ".";
                }
                else{
                    copyMap[i][j] = map[i][j];
                }
            }
        }
        map = copyMap;
    }
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            if(j != c-1){
            std::cout << map[i][j] << " ";
            }
            else{
                std::cout << map[i][j];
            }
        }
        std::cout << "\n";
    }
    return 0;
}