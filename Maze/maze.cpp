#include "maze.h"

Block::Block(){
    this->N = 1;
    this->S = 1;
    this->E = 1;
    this->W = 1;
    bool visited = false;
}

Block::Block(int north, int south, int east, int west, bool visit){
    this->N = north;
    this->S = south;
    this->E = east;
    this->W = west;
    this->visited = visit;
};

void Block::setWall(char side){
    switch (side)
    {
    case 'N':
        this -> N = 0;
        break;
    case 'S':
        this -> S = 0;
        break;
    case 'E':
        this -> E = 0;
        break;
    case 'W':
        this -> W = 0;
        break;
    default:
        break;
    }
    this->visited = true;
    return;
}

void Block::setWall_without_visit(char side){
    switch (side)
    {
    case 'N':
        this -> N = 0;
        break;
    case 'S':
        this -> S = 0;
        break;
    case 'E':
        this -> E = 0;
        break;
    case 'W':
        this -> W = 0;
        break;
    default:
        break;
    }
    return;    
}

std::string Block::get_walls(){
    int total = this->W + (this-> E)*2 + (this->S)*4 + (this->N)*8;
    std::string answer = std::to_string(total);
    return answer;
}

bool Block::get_visited(){
    return this->visited;
}