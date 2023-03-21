#ifndef MESH_H
#define MESH_H

#include "Vertice.h"
#include "Face.h"
using namespace std;

class Mesh{
    public: 
        // Constructors
        Mesh(string fileName, string extension);
        Mesh(vector<Vertice> verticeList, vector<Face> faceList): verticeList(verticeList), faceList(faceList){};
        
        // Import export
        void importFromOFF(string fileName);
        void exportToOFF(string shapeName, bool randomColors = false, bool plane = false);
        void exportToOFF(string shapeName, vector<float> values);

        // Print
        void printNeighbours(int verticeIndex);
        void getNeighboursList(int verticeIndex, vector<int>& neighboursList);
        void print();

        // Calcul diff
        Vertice computeLaplacian(int verticeIndex);
        float faceArea(Face face);

        // Delaunay
        void triangleSplit(int faceIndex, int verticeIndex);
        void outsideSplit(int faceIndex, int verticeIndex);
        void edgeFlip(int faceIndex1, int faceIndex2);
        int visibilityFind(int startIndex, Vertice vertice, bool verbose);
        void ImportFromTxt(string fileName);

        // Attributs
        vector<Vertice> verticeList;
        vector<Face> faceList;
        default_random_engine engine;
        uniform_real_distribution<double> uniform;
};

#endif