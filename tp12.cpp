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
    Mesh queen("queen", "off");
    queen.printNeighbours(0);
    vector<Vertice> laplacians = computeLaplacianMesh(queen);
    vector<float> laplaciansX = extractDimension(laplacians, "x");
    vector<float> laplaciansY = extractDimension(laplacians, "y");
    vector<float> laplaciansZ = extractDimension(laplacians, "z");
    vector<float> laplaciansNorms = extractDimension(laplacians, "norm");
    queen.exportToOFF("queen_with_laplacian_norms", laplaciansNorms);
    queen.exportToOFF("queen_with_laplacian_X", laplaciansX);
    queen.exportToOFF("queen_with_laplacian_Y", laplaciansY);
    queen.exportToOFF("queen_with_laplacian_Z", laplaciansZ);
    return 0;
};