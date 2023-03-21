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
    // Mesh doubleTetrahedre("double_tetrahedre", "off");

    // // === recherche par visibilité ===
    // cout << "trajet : " << doubleTetrahedre.visibilityFind(0, Vertice(1,-2,0), true) << endl;
    // cout << "trajet : " << doubleTetrahedre.visibilityFind(1, Vertice(1,-2,0), true) << endl;
    // cout << "trajet : " << doubleTetrahedre.visibilityFind(2, Vertice(1,-2,0), true) << endl;
    // cout << "trajet : " << doubleTetrahedre.visibilityFind(3, Vertice(1,-2,0), true) << endl;
    // cout << "trajet : " << doubleTetrahedre.visibilityFind(4, Vertice(1,-2,0), true) << endl;
    // cout << "trajet : " << doubleTetrahedre.visibilityFind(5, Vertice(1,-2,0), true) << endl;

    // // === isInCircle ===
    // cout << isInCircle(Vertice(0.2, 0.2, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> oui
    // cout << isInCircle(Vertice(5, 5, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> non
    // cout << isInCircle(Vertice(0.6, 0.1, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> oui
    // cout << isInCircle(Vertice(-0.6, 0.1, 0), Vertice(1, 0, 0), Vertice(0, 1, 0), Vertice(0,0,0)) << endl; // -> non
    
    // === construction par Delaunay ===
    Mesh mesh("alpes_poisson", "txt");
    mesh.exportToOFF("test_terrain", true, true);
    return 0;
};