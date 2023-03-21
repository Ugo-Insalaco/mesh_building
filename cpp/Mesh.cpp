#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "../h/Mesh.h"
#include "utils.cpp"

using namespace std;

Mesh::Mesh(string fileName, string extension, bool delaunay){
    uniform_real_distribution<double> uniform(0.0, 1.0);
    this->uniform = uniform;

    if(extension == "off"){
        importFromOFF(fileName);
    }
    if(extension == "txt"){
        importFromTxt(fileName, delaunay);
    }
};

void Mesh::importFromOFF(string fileName){
    ifstream myfile;
    myfile.open("off/"+fileName+".off");
    if(myfile.bad())
        {std::cout<<"Impossible d'ouvrir le fichier "<<fileName<<" en lecture \n"; exit(1);}
    float temp;
    map<pair<int, int>, pair<int, int>> faceMap;
    int verticeListSize;
    myfile >> verticeListSize;
    int faceListSize;
    myfile >> faceListSize;
    myfile >> temp;
    for(int i=0; i<verticeListSize; i++){
        float x, y, z;
        myfile >> x;
        myfile >> y;
        myfile >> z;
        Vertice vertice(x, y, z);
        verticeList.push_back(vertice);
    }
    for(int i=0; i<faceListSize; i++){
        int vertexNumber;
        myfile >> vertexNumber;
        if(vertexNumber != 3){
            throw "Non triangularisation";
        };
        int vi1, vi2, vi3;
        myfile >> vi1;
        myfile >> vi2;
        myfile >> vi3;
        verticeList[vi1].faceIndex = i;
        verticeList[vi2].faceIndex = i;
        verticeList[vi3].faceIndex = i;

        Face face(vi1, vi2, vi3);
        pair<int, int> fpi1(vi3, vi2);
        pair<int, int> fpi2(vi1, vi3);
        pair<int, int> fpi3(vi2, vi1);
        try
        {
            pair<int, int> v1 = faceMap.at(fpi1);
            int connectedFaceIndex = v1.first, connectedVerticeIndex = v1.second;
            faceList[connectedFaceIndex].connectFace(connectedVerticeIndex, i);
            face.connectFace(0, connectedFaceIndex);
        }
        catch(const std::out_of_range& e)
        {
            pair<int, int> fpi1p(vi2, vi3);
            pair<int, int> fpv1(i, 0);
            faceMap.insert(std::pair<pair<int, int>, pair<int, int>>(fpi1p, fpv1));
        }

        try
        {
            pair<int, int> v2 = faceMap.at(fpi2);
            int connectedFaceIndex = v2.first, connectedVerticeIndex = v2.second;
            faceList[connectedFaceIndex].connectFace(connectedVerticeIndex, i);
            face.connectFace(1, connectedFaceIndex);
        }
        catch(const std::out_of_range& e)
        {
            pair<int, int> fpi2p(vi3, vi1);
            pair<int, int> fpv2(i, 1);
            faceMap.insert(std::pair<pair<int, int>, pair<int, int>>(fpi2p, fpv2));
        }

        try
        {
            pair<int, int> v3 = faceMap.at(fpi3);
            int connectedFaceIndex = v3.first, connectedVerticeIndex = v3.second;
            faceList[connectedFaceIndex].connectFace(connectedVerticeIndex, i);
            face.connectFace(2, connectedFaceIndex);
        }
        catch(const std::out_of_range& e)
        {
            pair<int, int> fpi3p(vi1, vi2);
            pair<int, int> fpv3(i, 2);
            faceMap.insert(std::pair<pair<int, int>, pair<int, int>>(fpi3p, fpv3));
        }
        faceList.push_back(face);
    }
    cout << "imported" << endl;
    myfile.close();
};

void Mesh::exportToOFF(string shapeName, bool randomColors, bool plane){
    ofstream myfile;
    myfile.open ("off/"+shapeName+".off");
    myfile << verticeList.size() << " " << faceList.size() << " " << "0\n";
    for(Vertice vertice : verticeList){
        float z;
        if(plane && vertice.z >= 0){
            z = 0;
        }
        else{
        z = vertice.z;
    }
        myfile << vertice.x << " " << vertice.y << " " << z << "\n";
    }
    for(Face face : faceList){
        int * p = face.verticeIndexes;
        if(randomColors){
            myfile << "3 " << *p << " " << *(p+1) << " " << *(p+2) << " "<< (int)(this->uniform(this->engine)*255) << " " << (int)(this->uniform(this->engine)*255) << " " << (int)(this->uniform(this->engine)*255) <<"\n";
        }
        else{
            myfile << "3 " << *p << " " << *(p+1) << " " << *(p+2) << "\n";
        }
    }
    myfile.close();
};

void Mesh::exportToOFF(string shapeName, vector<float> values){
    ofstream myfile;
    int valuesSize = values.size();
    int verticeSize = verticeList.size();
    if(valuesSize != verticeSize){
        cout << "invalid values, must be same size as verticeList" << endl;
        return;
    };
    float max_value = *std::max_element(values.begin(), values.end());
    float min_value = *std::min_element(values.begin(), values.end());;

    float max_allowed = 100;
    float min_allowed = -100;
    vector<float>::iterator it;
    int i=0;
    for(it = values.begin(); it != values.end(); it++,i++ ){
        if(values[i] >=max_allowed){
            values[i] = max_allowed;
        }
        if(values[i] <=min_allowed){
            values[i] = min_allowed;
        }
        values[i] = (int)((values[i] - max(min_allowed, min_value))/(min(max_allowed, max_value) - max(min_allowed, min_value)) * 255);
    };

    myfile.open ("off/"+shapeName+".off");
    myfile << verticeList.size() << " " << faceList.size() << " " << "0\n";
    for(Vertice vertice : verticeList){
        myfile << vertice.x << " " << vertice.y << " " << vertice.z << "\n";
    }
    for(Face face : faceList){
        int * p = face.verticeIndexes;
        float value = values.at((*p));

        myfile << "3 " << *p << " " << *(p+1) << " " << *(p+2) << " "<< (value) << " " << 0 << " " << 0 <<"\n";
    }
    myfile.close();
};

void Mesh::print(){
    cout << "=== printing mesh ===" << endl;
    int faceListSize = faceList.size();
    int verticeListSize = verticeList.size();
    for(int i = 0; i< verticeListSize; i++){
        Vertice vertice = verticeList[i];
        cout << "=== Vertice " << i << " ===" << endl;
        cout << "x: " << vertice.x << ", y: " << vertice.y << ", z: " << vertice.z << endl;
        cout << "face index: " << vertice.faceIndex << endl;
    }
    for(int j = 0; j < faceListSize; j++){
        Face face = faceList[j];
        cout << "==== Face " << j << " ===" << endl;
        face.print();
    }
};

void Mesh::printNeighbours(int verticeIndex){
    int firstFaceIndex = verticeList.at(verticeIndex).faceIndex;
    int nextFaceIndex = firstFaceIndex;
    do
    {
        Face nextFace = faceList.at(nextFaceIndex);
        cout << "Face Suivante : " << nextFaceIndex << " " << nextFace.verticeIndexes[0] << " " << nextFace.verticeIndexes[1] << " " << nextFace.verticeIndexes[2]<< endl;
        int verticePosition;
        if(nextFace.verticeIndexes[0] == verticeIndex){
            verticePosition = 0;
        }
        if(nextFace.verticeIndexes[1] == verticeIndex){
            verticePosition = 1;
        }
        if(nextFace.verticeIndexes[2] == verticeIndex){
            verticePosition = 2;
        }
        nextFaceIndex = nextFace.faceIndexes[(verticePosition+1)%3];
    } while (nextFaceIndex != firstFaceIndex);
    
};

void Mesh::getNeighboursList(int verticeIndex, vector<int>& neighboursList){
    int firstFaceIndex = verticeList.at(verticeIndex).faceIndex;
    int nextFaceIndex = firstFaceIndex;
    do
    {
        Face nextFace = faceList.at(nextFaceIndex);
        int push = nextFaceIndex;
        neighboursList.push_back(push);
        int verticePosition;
        if(nextFace.verticeIndexes[0] == verticeIndex){
            verticePosition = 0;
        }
        if(nextFace.verticeIndexes[1] == verticeIndex){
            verticePosition = 1;
        }
        if(nextFace.verticeIndexes[2] == verticeIndex){
            verticePosition = 2;
        }
        nextFaceIndex = nextFace.faceIndexes[(verticePosition+1)%3];
    } while (nextFaceIndex != firstFaceIndex);
}

float Mesh::faceArea(Face face){
    Vertice v0 = verticeList.at(face.verticeIndexes[0]);
    Vertice v1 = verticeList.at(face.verticeIndexes[1]);
    Vertice v2 = verticeList.at(face.verticeIndexes[2]);
    Vertice crossVertice = cross(v1-v0, v2-v0);
    return crossVertice.normalize()/2;
}

Vertice Mesh::computeLaplacian(int verticeIndex){
    int firstFaceIndex = verticeList.at(verticeIndex).faceIndex;
    int nextFaceIndex = firstFaceIndex;
    int faceListSize = faceList.size();
    float areaSum = 0;
    float laplacianSumX = 0;
    float laplacianSumY = 0;
    float laplacianSumZ = 0;
    do
    {
        Face nextFace = faceList.at(nextFaceIndex);
        float area = faceArea(nextFace);

        areaSum += area/3;
        int verticePosition = nextFace.getVerticePosition(verticeIndex);
        Vertice v0 = verticeList.at(nextFace.verticeIndexes[verticePosition]);
        Vertice v1 = verticeList.at(nextFace.verticeIndexes[(verticePosition+1)%3]);
        Vertice v2 = verticeList.at(nextFace.verticeIndexes[(verticePosition+2)%3]);
        float cot1 = computeCot(v0-v1, v2-v1);

        nextFaceIndex = nextFace.faceIndexes[(verticePosition+1)%3];
        Face nextNextFace = faceList.at(nextFaceIndex);
        int nextVerticePosition = nextNextFace.getVerticePosition(verticeIndex);
        v0 = verticeList.at(nextNextFace.verticeIndexes[nextVerticePosition]);
        v1 = verticeList.at(nextNextFace.verticeIndexes[(nextVerticePosition+1)%3]);
        v2 = verticeList.at(nextNextFace.verticeIndexes[(nextVerticePosition+2)%3]);
        float cot2 = computeCot(v0-v2, v1-v2);

        laplacianSumX += (cot1 + cot2)*(v1.x - v0.x);
        laplacianSumY += (cot1 + cot2)*(v1.y - v0.y);
        laplacianSumZ += (cot1 + cot2)*(v1.z - v1.z);

    } while (nextFaceIndex != firstFaceIndex);
    return Vertice(laplacianSumX/(areaSum*2), laplacianSumY/(areaSum*2), laplacianSumZ/(areaSum*2));
}

void Mesh::triangleSplit(int faceIndex, int verticeIndex){
    Face face = faceList.at(faceIndex);
    Vertice vertice = verticeList.at(verticeIndex);

    // On créé les nouvelles faces
    Face newFace0(face.verticeIndexes[1], face.verticeIndexes[2], verticeIndex);
    Face newFace1(face.verticeIndexes[2], face.verticeIndexes[0], verticeIndex);
    Face newFace2(face.verticeIndexes[0], face.verticeIndexes[1], verticeIndex);

    // On ajoute les nouvelles faces à la liste et on récupère leurs indexes;
    int faceIndex1 = faceList.size();
    int faceIndex2 = faceList.size() + 1;

    // On modifie les indices des faces associées à chaque sommet puis on ajoute la nouvelle face
    verticeList[verticeIndex].faceIndex = faceIndex;
    verticeList[face.verticeIndexes[0]].faceIndex = faceIndex1;
    verticeList[face.verticeIndexes[1]].faceIndex = faceIndex2;
    verticeList[face.verticeIndexes[2]].faceIndex = faceIndex;

    // On met à jour le faces existantes pour lier au nouvelles faces (pas besoin de changer l'indice
    // de la première face)
    faceList.at(face.faceIndexes[1]).updateFaceIndex(faceIndex, faceIndex1);
    faceList.at(face.faceIndexes[2]).updateFaceIndex(faceIndex, faceIndex2);

    // On connecte les nouvelles faces entre elles
    newFace0.connectFace(0, faceIndex1);
    newFace0.connectFace(1, faceIndex2);
    newFace0.connectFace(2, face.faceIndexes[0]);

    newFace1.connectFace(0, faceIndex2);
    newFace1.connectFace(1, faceIndex);
    newFace1.connectFace(2, face.faceIndexes[1]);

    newFace2.connectFace(0, faceIndex);
    newFace2.connectFace(1, faceIndex1);
    newFace2.connectFace(2, face.faceIndexes[2]);

    // On met à jour la face existante et on ajoute les nouvelles faces
    faceList[faceIndex] = newFace0;
    faceList.push_back(newFace1);
    faceList.push_back(newFace2);
}

void Mesh::outsideSplit(int faceIndex, int verticeIndex){
    Face face = faceList.at(faceIndex);
    Vertice vertice = verticeList.at(verticeIndex);

    // S'il y a déjà une face avec le sommet alors il faut la flipper
    bool createfp0 = faceList.at(face.faceIndexes[0]).getVerticePosition(verticeIndex) < 0;
    bool createfp1 = faceList.at(face.faceIndexes[1]).getVerticePosition(verticeIndex) < 0;
    bool createfp2 = faceList.at(face.faceIndexes[2]).getVerticePosition(verticeIndex) < 0;
    
    int faceIndex1, faceIndex2;
    if(!createfp0){
        edgeFlip(faceIndex, face.faceIndexes[0]);
    }
    if(!createfp1){
        edgeFlip(faceIndex, face.faceIndexes[1]);
    }
    if(!createfp2){
        edgeFlip(faceIndex, face.faceIndexes[2]);
    }
    if((!createfp0 && !createfp1) || (!createfp0 && !createfp2) || (!createfp2 && !createfp1)){
        cout << "problem" << endl;
    }
    if(createfp0 && createfp1 && createfp2){
        // On créé les nouvelles faces
        Face newFace0(face.verticeIndexes[1], face.verticeIndexes[2], verticeIndex);
        Face newFace1(face.verticeIndexes[2], face.verticeIndexes[0], verticeIndex);
        Face newFace2(face.verticeIndexes[0], face.verticeIndexes[1], verticeIndex);

        // On ajoute les nouvelles faces à la liste et on récupère leurs indexes;
        int faceIndex1 = faceList.size();
        int faceIndex2 = faceList.size() + 1;

        // On modifie les indices des faces associées à chaque sommet puis on ajoute la nouvelle face
        verticeList[verticeIndex].faceIndex = faceIndex;
        verticeList[face.verticeIndexes[0]].faceIndex = faceIndex1;
        verticeList[face.verticeIndexes[1]].faceIndex = faceIndex2;
        verticeList[face.verticeIndexes[2]].faceIndex = faceIndex;

        // On met à jour le faces existantes pour lier au nouvelles faces (pas besoin de changer l'indice
        // de la première face)
        faceList.at(face.faceIndexes[1]).updateFaceIndex(faceIndex, faceIndex1);
        faceList.at(face.faceIndexes[2]).updateFaceIndex(faceIndex, faceIndex2);

        // On connecte les nouvelles faces entre elles
        newFace0.connectFace(0, faceIndex1);
        newFace0.connectFace(1, faceIndex2);
        newFace0.connectFace(2, face.faceIndexes[0]);

        newFace1.connectFace(0, faceIndex2);
        newFace1.connectFace(1, faceIndex);
        newFace1.connectFace(2, face.faceIndexes[1]);

        newFace2.connectFace(0, faceIndex);
        newFace2.connectFace(1, faceIndex1);
        newFace2.connectFace(2, face.faceIndexes[2]);

        // On met à jour la face existante et on ajoute les nouvelles faces
        faceList[faceIndex] = newFace0;
        faceList.push_back(newFace1);
        faceList.push_back(newFace2);
    }
}

void Mesh::edgeFlip(int faceIndex1, int faceIndex2){
    Face face1 = faceList.at(faceIndex1);
    Face face2 = faceList.at(faceIndex2);

    // On récupère les indices de l'arrête en commun
    int edgeIndex1, edgeIndex2;
    bool invert = false;
    getCommonEdge(face1, face2, edgeIndex1, edgeIndex2, invert);
    if(edgeIndex1 == -1 || edgeIndex2 == -1){
        return;
    }

    // On créé les nouvelles faces avec les bons indices de sommets
    Face newFace1, newFace2;
    if(!invert){
        newFace1.updateVertices(face1.verticeIndexes[(edgeIndex1+1)%3], face1.verticeIndexes[(edgeIndex1+2)%3], face2.verticeIndexes[(edgeIndex2+1)%3]);
        newFace2.updateVertices(face2.verticeIndexes[(edgeIndex2+1)%3], face1.verticeIndexes[(edgeIndex1+2)%3], face1.verticeIndexes[(edgeIndex1)]);
    }
    else{
        newFace1.updateVertices(face1.verticeIndexes[(edgeIndex1)%3], face1.verticeIndexes[(edgeIndex1+1)%3], face2.verticeIndexes[(edgeIndex2+2)%3]);
        newFace2.updateVertices(face2.verticeIndexes[(edgeIndex2+2)%3], face1.verticeIndexes[(edgeIndex1+1)%3], face1.verticeIndexes[(edgeIndex1+2)%3]);
    }

    // On connecte les faces entre elles et met à jour les faces qui changent
    if(!invert){
        newFace1.connectFace(0, faceIndex2);
        newFace1.connectFace(1, face2.faceIndexes[(edgeIndex2)]);
        newFace1.connectFace(2, face1.faceIndexes[(edgeIndex1)]);

        newFace2.connectFace(0, face1.faceIndexes[(edgeIndex1+1)%3]);
        newFace2.connectFace(1, face2.faceIndexes[(edgeIndex2+2)%3]);
        newFace2.connectFace(2, faceIndex1);

        faceList[face1.faceIndexes[(edgeIndex1+1)%3]].updateFaceIndex(faceIndex1, faceIndex2);
        faceList[face2.faceIndexes[(edgeIndex2)]].updateFaceIndex(faceIndex2, faceIndex1);

        // On remet à jour les faces associées à chaque sommet
        verticeList[face2.verticeIndexes[(edgeIndex2+1)%3]].faceIndex = faceIndex2;
        verticeList[face1.verticeIndexes[edgeIndex1]].faceIndex = faceIndex2;
        verticeList[face1.verticeIndexes[(edgeIndex1+1)%3]].faceIndex = faceIndex1;
        verticeList[face1.verticeIndexes[(edgeIndex1+2)%3]].faceIndex = faceIndex1;
    }
    else{
        newFace1.connectFace(0, faceIndex2);
        newFace1.connectFace(1, face2.faceIndexes[(edgeIndex2+1)%3]);
        newFace1.connectFace(2, face1.faceIndexes[(edgeIndex1+2)%3]);

        newFace2.connectFace(0, face1.faceIndexes[(edgeIndex1)%3]);
        newFace2.connectFace(1, face2.faceIndexes[(edgeIndex2)%3]);

        newFace2.connectFace(2, faceIndex1);

        faceList[face1.faceIndexes[(edgeIndex1)%3]].updateFaceIndex(faceIndex1, faceIndex2);
        faceList[face2.faceIndexes[(edgeIndex2+1)%3]].updateFaceIndex(faceIndex2, faceIndex1);

        // On remet à jour les faces associées à chaque sommet
        verticeList[face2.verticeIndexes[(edgeIndex2+2)%3]].faceIndex = faceIndex2;
        verticeList[face1.verticeIndexes[edgeIndex1]].faceIndex = faceIndex1;
        verticeList[face1.verticeIndexes[(edgeIndex1+1)%3]].faceIndex = faceIndex1;
        verticeList[face1.verticeIndexes[(edgeIndex1+2)%3]].faceIndex = faceIndex2;
    }

    // On met à jour les nouvelles faces
    faceList[faceIndex1] = newFace1;
    faceList[faceIndex2] = newFace2;
} 

int Mesh::visibilityFind(int startIndex, Vertice vertice){
    // On choisit une face au hasard
    // Si la face contient le point c'est bon
    // On cherche l'arrête qui donne sur le nouveau point par un test d'orientation (on en choisit une au
    // hasard s'il y en a plusieurs et on choisit l'arrête qui donne sur une face finie si on est sur
    // une face infinie)
    // Si l'arrête donne sur une face à l'infini
        // Alors on tourne autours du point infini pour trouver les faces que le point voit
        // Pour chaque face infinie vues
        // On la split avec notre nouveau point
    // Sinon on peut ajouter le point comme on fait habituellement
    // On suppose que s'il y a un point à l'infini, alors il est à l'indexe 0
    // int current_face_index = rand() % (faceList.size()-1) + 1;
    int current_face_index = startIndex;
    Face current_face = faceList.at(current_face_index);
    // Si la face contient le point à l'infini on se ramène sur une face de base
    if(current_face.isInfinite()){
        int infinite_position = current_face.getVerticePosition(0);
        current_face_index = current_face.faceIndexes[infinite_position];
    }
    bool found = false;
    int iter = 0;
    while(!found && iter < 10000){
        iter++;
        current_face = faceList.at(current_face_index);
        // Si on revient sur une face infinie c'est qu'on est sorti
        if(current_face.isInfinite()){
            break;
        }

        // On calcule l'orientation du nouveau point par rapport aux trois arrêtes du triangle
        Vertice v0 = verticeList[current_face.verticeIndexes[0]];
        Vertice v1 = verticeList[current_face.verticeIndexes[1]];
        Vertice v2 = verticeList[current_face.verticeIndexes[2]];

        double pred0 = orientationTest(v1, v2, vertice);
        double pred1 = orientationTest(v2, v0, vertice);
        double pred2 = orientationTest(v0, v1, vertice);
        
        // Si toutes les orientations sont bonnes on est à l'intérieur
        if(pred0 < 0 && pred1 < 0 && pred2 < 0){
            found = true;
        }

        // Sinon on cherche par quelle face sortir pour se diriger vers le bon triangle
        else if(pred0 > 0){
            current_face_index = current_face.faceIndexes[0];
        }
        else if(pred1 > 0){
            current_face_index = current_face.faceIndexes[1];
        }
        else if(pred2 > 0){
            current_face_index = current_face.faceIndexes[2];
        }
    }
    return current_face_index;

}

void Mesh::propagateSplit(int verticeIndex){
    vector<int> facesToTest;

    // On commence par regarder tous les voisins
    getNeighboursList(verticeIndex, facesToTest);
    int i = 0;

    while(!facesToTest.empty() && i < 1000){
        int faceIndex = facesToTest.back();
        Face face = faceList.at(faceIndex);

        // Si la face actuelle est infinie on ne veut pas la flipper
        if(face.isInfinite()){
            facesToTest.pop_back();
            continue;
        }

        // On récupère la face opposée 
        int verticePosition = face.getVerticePosition(verticeIndex);
        int oppositeFaceIndex = face.faceIndexes[verticePosition];
        Face oppositeFace = faceList.at(oppositeFaceIndex);
        int oppositeFacePosition = oppositeFace.getFacePosition(faceIndex);
        
        // Si la face opposée est infinie on ne veut pas la flipper non plus
        if(oppositeFace.isInfinite()){
            facesToTest.pop_back();
            continue;
        }

        // On test si le triangle est Delaunay par rapport à la face opposée
        Vertice v1 = verticeList.at(oppositeFace.verticeIndexes[oppositeFacePosition]);
        Vertice v2 = verticeList.at(face.verticeIndexes[(verticePosition)]);
        Vertice v3 = verticeList.at(face.verticeIndexes[(verticePosition+1)%3]);
        Vertice v4 = verticeList.at(face.verticeIndexes[(verticePosition+2)%3]);
        
        // Si c'est le cas alors on flip les faces et on les ajoute à notre pile
        if(isInCircle(v1, v2, v3, v4) >= 0){
            edgeFlip(faceIndex, oppositeFaceIndex);
            facesToTest.push_back(oppositeFaceIndex);
        }

        // Sinon on passe à la suivante
        else{
            facesToTest.pop_back();
        }
        i++;

    }
}
void Mesh::importFromTxt(string fileName, bool delaunay){
    ifstream myfile;
    int verticeListSize;
    myfile.open("txt/"+fileName+".txt");
    if(myfile.bad())
        {std::cout<<"Impossible d'ouvrir le fichier "<<fileName<<" en lecture \n"; exit(1);}
    myfile >> verticeListSize;
    Vertice v0(0,0,-2000);
    v0.faceIndex = 1;
    verticeList.push_back(v0);

    // On récupère les trois premiers points et on en fait un tétrahèdre avec le point à l'infini
    float x, y, z;
    myfile >> x;
    myfile >> y;
    myfile >> z;
    Vertice v1(x, y, z);
    myfile >> x;
    myfile >> y;
    myfile >> z;
    Vertice v2(x, y, z);
    myfile >> x;
    myfile >> y;
    myfile >> z;
    Vertice v3(x, y, z);

    v1.faceIndex = 0;
    v2.faceIndex = 0;
    v3.faceIndex = 0;

    verticeList.push_back(v1);
    verticeList.push_back(v2);
    verticeList.push_back(v3);

    int vi1 = 1, vi2 = 2, vi3 = 3, vi4 = 0;
    Face f0(vi1, vi2, vi3);
    Face f1(vi1, vi4, vi2);
    Face f2(vi1, vi3, vi4);
    Face f3(vi2, vi4, vi3);
    connectFacesTetrahedre(f0, f1, f2, f3, 0, 1, 2, 3);
    faceList.push_back(f0);
    faceList.push_back(f1);
    faceList.push_back(f2);
    faceList.push_back(f3);

    // Pour chaque nouveau point
    for(int i=0; i<verticeListSize - 3; i++){
        float x, y, z;
        myfile >> x;
        myfile >> y;
        myfile >> z;
        Vertice vertice(x, y, z);
        int faceIndex = visibilityFind(0, vertice);
        verticeList.push_back(vertice);
        int verticeIndex = verticeList.size() - 1;

        // Si on trouve une face dans le maillage qui contient le point, on la split
        if(!faceList[faceIndex].isInfinite()){
            triangleSplit(faceIndex, verticeIndex);
            if(delaunay){
                propagateSplit(verticeIndex);
            }
        }
        // Sinon on tourne autours du sommet infini et on split toutes les faces infinies que l'on voit
        else{
            vector<int> neighboursList;
            getNeighboursList(0, neighboursList);
            bool previousVisible;
            bool lastVisible = false;
            bool firstVisible = false;
            int startIndex =-1;
            for(unsigned j = 0; j<neighboursList.size(); j++){
                int nextFaceIndex = neighboursList[j];
                Face nextFace = faceList.at(nextFaceIndex);

                // On cherche quelle est la première face infinie que l'on peut split ou flip
                // Afin de ne pas se retrouver coincé entre deux nouvelles faces infinies
                if(nextFace.getVerticePosition(verticeIndex) >= 0){
                    continue;
                }
                int verticePosition = nextFace.getVerticePosition(0);
                Vertice v1 = verticeList[nextFace.verticeIndexes[(verticePosition+1)%3]];
                Vertice v2 = verticeList[nextFace.verticeIndexes[(verticePosition+2)%3]];
                double pred = orientationTest(v1, v2, vertice);
                if(j==neighboursList.size() - 1 && pred < 0){
                    lastVisible = true;
                }
                if(j==0 && pred < 0){
                    firstVisible = true;
                }
                if(pred < 0 && !previousVisible){
                    startIndex = j;
                }
                previousVisible = pred<0;
            };
            if(firstVisible && !lastVisible){
                startIndex = 0;
            }

            // Maintenant on parcourt toutes les faces infinies
            int neighboursListSize = neighboursList.size();
            for(unsigned j = 0; j<neighboursList.size(); j++){
                int nextFaceIndex = neighboursList[(j+startIndex)%(neighboursListSize)];
                Face nextFace = faceList.at(nextFaceIndex);
                if(nextFace.getVerticePosition(verticeIndex) >= 0){
                    continue;
                }
                int verticePosition = nextFace.getVerticePosition(0);
                Vertice v1 = verticeList[nextFace.verticeIndexes[(verticePosition+1)%3]];
                Vertice v2 = verticeList[nextFace.verticeIndexes[(verticePosition+2)%3]];
                double pred = orientationTest(v1, v2, vertice);
                if(pred < 0){
                    outsideSplit(nextFaceIndex, verticeIndex);
                    if(delaunay){
                        propagateSplit(verticeIndex);
                    }       
                }
            };
        }
    }
}