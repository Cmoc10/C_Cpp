#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>

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
int main(int argc, char** argv){
    std::stack<std::string> op_stack;
    std::stack<std::string> num_stack;
    std::string equation = argv[1];
    std::vector<std::string> parts;
    std::istringstream ss(equation);
    std::string temp;
    std::string operations = "()^*/-+";
    std::string precedent_1 = "^*-/+";
    std::string precedent_2 = "^*/";
    bool in_paren = false;
    while(std::getline(ss, temp, ' ')){
        parts.push_back(temp);
    }
    for(int i = 0; i<parts.size(); i++){
        if(is_in(parts[i], operations)){
            if(parts[i] == "("){
                op_stack.push(parts[i]);
                in_paren = true;
            }
            else if(parts[i] == ")"){
                while(op_stack.top() != "("){
                    std::cout << op_stack.top() << " ";
                    op_stack.pop();
                }
                op_stack.pop();
                in_paren = false;
            }
            else if(in_paren && parts[i] == "^"){
                op_stack.push(parts[i]);
            }
            else{
                if(parts[i] == "^"){
                    while(!op_stack.empty() && op_stack.top() == "^"){
                        std::cout << op_stack.top() << " ";
                        op_stack.pop();
                    }
                }
                else if(parts[i] == "*" || parts[i] == "/"){
                    while(!op_stack.empty() && is_in(op_stack.top(), precedent_2)){
                        std::cout << op_stack.top() << " ";
                        op_stack.pop();
                    }
                }
                else{
                    while(!op_stack.empty() && is_in(op_stack.top(), precedent_1)){
                        std::cout << op_stack.top() << " ";
                        op_stack.pop();
                    }
                }
                op_stack.push(parts[i]);
            }
        }
        else{
            std::cout << parts[i] << " ";
        }
    }
    while(!op_stack.empty()){
        std::cout << op_stack.top() << " ";
        op_stack.pop();
    }
    std::cout << "\n";
    return 0;
}