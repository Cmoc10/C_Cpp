#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

int main(){
    std::vector<std::vector<int>> board (3, std::vector<int>(3));
    std::vector<int> answer;
    char input;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
        int temp;
        std::cin >> temp;
        board[i][j] = temp;
        }
    }
    while(std::cin >> input){
        int temp;
        int iindex;
        int jindex;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j] == 0){
                    iindex = i;
                    jindex = j;
                }
            }
        }
        if(input == 'U'){
        if(iindex != 0){
            temp = board[iindex-1][jindex];
            board[iindex-1][jindex] = board[iindex][jindex];
            board[iindex][jindex] = temp;
        }
        }
        if(input == 'D'){
        if(iindex != 2){
            temp = board[iindex+1][jindex];
            board[iindex+1][jindex] = board[iindex][jindex];
            board[iindex][jindex] = temp;
        }
        }
        if(input == 'R'){
        if(jindex != 2){
            temp = board[iindex][jindex+1];
            board[iindex][jindex+1] = board[iindex][jindex];
            board[iindex][jindex] = temp;  
        }
        }
        if(input == 'L'){
        if(jindex != 0){
            temp = board[iindex][jindex-1];
            board[iindex][jindex-1] = board[iindex][jindex];
            board[iindex][jindex] = temp;
        }
        }
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            answer.push_back(board[i][j]);
        }
    }
    for(int i=0; i<7; i++){
        if(answer[0] == 0){
           std::cout << "Wrong solution!" << "\n"; 
           return 0;
        }
        if(answer[i] > answer[i+1]){
            std::cout << "Wrong solution!" << "\n";
            return 0;
        }
    }

    std::cout << "Solution is good!" << "\n";
    return 0;
}