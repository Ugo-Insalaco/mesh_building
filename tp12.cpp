#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "cpp/Vertice.cpp"
#include "cpp/Face.cpp"
#include "cpp/Mesh.cpp"

using namespace std;

int main(){
    Mesh queen("queen");
    queen.printNeighbours(0);
    vector<float> laplacians = computeLaplacianMesh(queen);
    queen.exportToOFF("queen_with_laplacian", laplacians);
    int wait;
    cin >> wait;
    return 0;
};