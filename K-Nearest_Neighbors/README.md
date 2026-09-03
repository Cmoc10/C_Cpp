K-Nearest Neighbors Classifier

A simple C++ implementation of the K-Nearest Neighbors (KNN) algorithm. The program classifies a test point as either Red (R) or Blue (B) based on the colors of its nearest neighboring points.

Features
Represents points using x and y coordinates.
Calculates the Euclidean distance between points.
Supports two classes:
R — Red
B — Blue
Finds the specified number of nearest neighbors.
Determines the test point's class based on the majority of its neighbors.
Requirements
C++ compiler supporting C++11 or later.
How to Compile
g++ main.cpp -o knn

How to Run
./knn


The program expects the number of training points, the number of neighbors to consider, the training points, and then the test point.

Input Format

The first line contains:

number_of_points number_of_neighbors


Each training point is then provided as:

x y color


The final point is the point that needs to be classified:

x y

Example Input
5 3
1 1 R
2 2 R
8 8 B
9 9 B
7 8 B
3 3


In this example:

There are 5 training points.
The program considers the 3 nearest neighbors.
The point (3, 3) is the test point.
Output

The program prints the predicted class:

R


or:

B

How It Works

The program follows the basic K-Nearest Neighbors algorithm:

Reads the training points and their colors.
Reads the test point.
Calculates the Euclidean distance between the test point and each training point.
Finds the closest neighbors.
Counts how many of the nearest neighbors are Red and Blue.
Classifies the test point based on the majority class.

The Euclidean distance is calculated using:

distance = √((x₁ - x₂)² + (y₁ - y₂)²)

Classification Rule

The final classification is based on the number of neighbors:

If there are more Blue neighbors, the result is B.
Otherwise, the result is R.

If there is a tie, the program chooses R.

Note

This is a basic implementation of KNN designed for two-dimensional points and two possible classes. It does not use any external machine-learning libraries; all distance calculations and neighbor selection are performed manually.
