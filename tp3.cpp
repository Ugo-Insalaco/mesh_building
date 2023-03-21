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
    // ==== Ajout d'un point par triangleSplit ====
    // Mesh triangle("tetrahedre");
    // triangle.print();
    // triangle.triangleSplit(0, Vertice(sqrt(2)/4,sqrt(2)/4,-0.5));
    // triangle.exportToOFF("tetrahedre_with_split");
    // triangle.printNeighbours(0);

    //==== Ajout d'un point par triangleSplit en dehors d'une face ====
    Mesh triangle("tetrahedre", "off");
    triangle.print();
    triangle.triangleSplit(0, Vertice(2,2,-1));
    triangle.exportToOFF("tetrahedre_with_split_outside");
    triangle.printNeighbours(0);

    //==== Changement d'une face par edgeFlip ====
    Mesh doubleTetrahedre("double_tetrahedre", "off");
    doubleTetrahedre.edgeFlip(0, 1);
    doubleTetrahedre.exportToOFF("double_tetrahedre_with_flip");
    doubleTetrahedre.printNeighbours(1);
    doubleTetrahedre.print();

    // ==== Test d'orientation et d'appartenance ====
    // Vertice v1(0,0,0);
    // Vertice v2(1,0,0);
    // Vertice v3(0,1,0);
    // Vertice vIn(0.1, 0.1, 0);
    // Vertice vOut(1, 1, 0);
    // cout << orientationTest(v1, v2, v3) << endl; // -> 1
    // cout << orientationTest(v1, v3, v2) << endl; // -> -1
    // cout << verticeInTriangle(vIn, v1, v2, v3) << endl; // -> 1
    // cout << verticeInTriangle(vOut, v1, v2, v3) << endl; // -> -1
    return 0;
};