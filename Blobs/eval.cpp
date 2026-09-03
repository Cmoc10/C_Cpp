#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <cmath>

bool is_in(std::string check, std::string base){
    std::string temp;
    for(int i=0; i<base.size(); i++){
        temp = base[i];
        if(check == temp){
            return true;
        }
    }
    return false;
}

int compute(float num1, float num2, std::string operation){
    int total;
    if(operation == "/"){
        total = floor(num1/num2);
    }
    else if(operation == "*"){
        total = num1*num2;
    }
    else if(operation == "+"){
        total = num1+num2;
    }
    else if(operation == "-"){
        total = num1-num2;
    }
    else if(operation == "^"){
        total = pow(num1, num2);
    }
    return total;
}

int main(int argc, char** argv){
    std::stack<std::string> num_stack;
    std::string equation = argv[1];
    std::vector<std::string> parts;
    std::istringstream ss(equation);
    std::string temp;
    std::string operations = "^*/-+";
    while(std::getline(ss, temp, ' ')){
        parts.push_back(temp);
    }
    int total;
    float num1, num2;
    for(int i = 0; i<parts.size(); i++){
        if(is_in(parts[i], operations)){
            num2 = stof(num_stack.top());
            num_stack.pop(); 
            num1 = stof(num_stack.top());
            num_stack.pop();
            num_stack.push(std::to_string(compute(num1, num2, parts[i])));
        }
        else{
            num_stack.push(parts[i]);
        }
    }
    total = stoi(num_stack.top());
    std::cout << total <<"\n";
    return 0;
}