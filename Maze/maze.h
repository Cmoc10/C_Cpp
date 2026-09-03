#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>


class Block{
    private:
        int N;
        int S;
        int E;
        int W;
        bool visited;
    public:
        Block();
        Block(int north, int south, int east, int west, bool visit);
        bool get_visited();
        void setWall_without_visit(char side);
        void setWall(char side);
        std::string get_walls();

};