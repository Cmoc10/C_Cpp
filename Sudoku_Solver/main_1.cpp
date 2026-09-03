#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

int main() {
    std::vector<std::vector<int>> matrix(9, std::vector<int>(9));
    int num, count = 0;
    int i = 0;
    while (std::cin >> num) {
        if (count == 9) {
            count = 0;
            i++;
        }
        matrix[i][count] = num;
        count++;
    }

    // Check rows
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            for (int k = 0; k < matrix[i].size(); k++) {
                if ((matrix[i][j] == matrix[i][k] && k != j) || matrix[i][j] > 9 || matrix[i][j] < 1) {
                    std::cout << "Wrong solution!";
                    return 0;
                }
            }
        }
    }

    // Check boxes
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            std::vector<int> seen(10, 0);
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    int num = matrix[i + k][j + l];
                    if (num < 1 || num > 9 || seen[num] == 1) {
                        std::cout << "Wrong solution!";
                        return 0;
                    }
                    seen[num] = 1;
                }
            }
        }
    }

    std::cout << "Solution is good!" << std::endl;
    return 0;
}
