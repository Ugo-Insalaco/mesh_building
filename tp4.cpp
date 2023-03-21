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
    Mesh doubleTetrahedre("double_tetrahedre", "off");

    // === recherche par visibilité ===
    cout << "trajet : " << doubleTetrahedre.visibilityFind(0, Vertice(1,-2,0)) << endl;
    cout << "trajet : " << doubleTetrahedre.visibilityFind(1, Vertice(1,-2,0)) << endl;
    cout << "trajet : " << doubleTetrahedre.visibilityFind(2, Vertice(1,-2,0)) << endl;
    cout << "trajet : " << doubleTetrahedre.visibilityFind(3, Vertice(1,-2,0)) << endl;
    cout << "trajet : " << doubleTetrahedre.visibilityFind(4, Vertice(1,-2,0)) << endl;
    cout << "trajet : " << doubleTetrahedre.visibilityFind(5, Vertice(1,-2,0)) << endl;

    // === isInCircle ===
    cout << isInCircle(Vertice(0.2, 0.2, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> oui
    cout << isInCircle(Vertice(5, 5, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> non
    cout << isInCircle(Vertice(0.6, 0.1, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> oui
    cout << isInCircle(Vertice(-0.6, 0.1, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> non
    
    // === construction par Delaunay ===
    Mesh mesh("test_terrain", "txt");
    mesh.exportToOFF("test_terrain_delaunay", true, true);

    Mesh mesh2("test_terrain", "txt", false);
    mesh2.exportToOFF("test_terrain_no_delaunay", true, true);
    return 0;
};