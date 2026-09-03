#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

struct point{
    double x;
    double y;

double euDistance (point p2){
    double distance = 0;
    distance = sqrt(pow((x-p2.x),2) + pow((y-p2.y),2));
    return distance;
}
};

int main(){
    std::vector<point> redPoints;
    std::vector<point> bluePoints;
    int classPoints, neighborNum;
    point testPoint;
    std::cin >> classPoints; 
    std::cin >> neighborNum;
    for(int i=0; i<=classPoints; i++){
        double xTemp;
        double yTemp;
        std::string tempColor = "0";
        std::cin >> xTemp; 
        std::cin >> yTemp;
        if(i < classPoints){
            std::cin >> tempColor;
        }
        point tempStruct;
        tempStruct.x = xTemp;
        tempStruct.y = yTemp;
        if(i == classPoints){
            testPoint = tempStruct;
        }
        if (tempColor == "R"){
            redPoints.push_back(tempStruct);
        }
        else if (tempColor == "B"){
            bluePoints.push_back(tempStruct);
        }
    }
    int numNeighbors = 0;
    int redNeighbors = 0;
    int blueNeighbors = 0;
    while(numNeighbors < neighborNum){
        int minIndex = 0;
        std::string minColor;
        double minDistance = 0;
        for(int i=0; i<redPoints.size(); i++){
            if(testPoint.euDistance(redPoints[i]) < minDistance || i==0){
                minIndex = i;
                minDistance = testPoint.euDistance(redPoints[i]);
                minColor = "R";
            }
        }
        for(int i=0; i<bluePoints.size(); i++){
            if(testPoint.euDistance(bluePoints[i]) < minDistance){
                minIndex = i;
                minDistance = testPoint.euDistance(bluePoints[i]);
                minColor = "B";
            }
        }
        if(redPoints.size() == 0){
            minColor = "B";
        }
        if(bluePoints.size() == 0){
            minColor = "R";
        }
        if(minColor == "R"){
            point temp = redPoints[redPoints.size()-1];
            redPoints[redPoints.size()-1] = redPoints[minIndex];
            redPoints[minIndex] = temp;
            redPoints.pop_back();
            redNeighbors++;
        }
        else if(minColor == "B"){
            point temp = bluePoints[bluePoints.size()-1];
            bluePoints[bluePoints.size()-1] = bluePoints[minIndex];
            bluePoints[minIndex] = temp;
            bluePoints.pop_back();
            blueNeighbors++;
        }
        numNeighbors++;
    }
    if(blueNeighbors > redNeighbors){
        std::cout << "B" << "\n";
    }
    else{
        std::cout << "R" << "\n";
    }
    return 0;
}