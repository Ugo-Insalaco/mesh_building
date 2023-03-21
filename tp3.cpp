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
    Mesh triangle("tetrahedre", "off");
    triangle.print();
    Vertice v(sqrt(2)/4,sqrt(2)/4,-0.5);
    triangle.verticeList.push_back(v);
    int index = triangle.verticeList.size() -1;
    triangle.triangleSplit(0, index);
    triangle.exportToOFF("tetrahedre_with_split", true);
    triangle.printNeighbours(0);

    //==== Ajout d'un point par triangleSplit en dehors d'une face ====
    Mesh triangle2("tetrahedre", "off");
    triangle2.print();
    Vertice w(2,2,-1);
    triangle2.verticeList.push_back(w);
    int index2 = triangle2.verticeList.size() -1;
    triangle2.outsideSplit(0, index2);
    triangle2.exportToOFF("tetrahedre_with_split_outside", true);
    triangle2.printNeighbours(0);

    //==== Changement d'une face par edgeFlip ====
    Mesh doubleTetrahedre("double_tetrahedre", "off");
    doubleTetrahedre.edgeFlip(0, 1);
    doubleTetrahedre.exportToOFF("double_tetrahedre_with_flip", true);
    doubleTetrahedre.printNeighbours(1);
    doubleTetrahedre.print();

    // ==== Test d'orientation et d'appartenance ====
    Vertice v1(0,0,0);
    Vertice v2(1,0,0);
    Vertice v3(0,1,0);
    Vertice vIn(0.1, 0.1, 0);
    Vertice vOut(1, 1, 0);
    cout << orientationTest(v1, v2, v3) << endl; // -> 1
    cout << orientationTest(v1, v3, v2) << endl; // -> -1
    cout << verticeInTriangle(vIn, v1, v2, v3) << endl; // -> 1
    cout << verticeInTriangle(vOut, v1, v2, v3) << endl; // -> -1
    return 0;
};