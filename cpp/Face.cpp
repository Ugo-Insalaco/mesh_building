#include "../h/Face.h"

Face::Face(int index1, int index2, int index3){
    verticeIndexes[0] = index1;
    verticeIndexes[1] = index2;
    verticeIndexes[2] = index3;
};

Face::Face(){
    verticeIndexes[0] = -1;
    verticeIndexes[1] = -1;
    verticeIndexes[2] = -1;
}
void Face::updateVertices(int index1, int index2, int index3){
    verticeIndexes[0] = index1;
    verticeIndexes[1] = index2;
    verticeIndexes[2] = index3;
};

void Face::connectFace(int verticeIndex, int faceIndex){
    faceIndexes[verticeIndex] = faceIndex;
    return;
};

void Face::updateFaceIndex(int oldFaceIndex, int newFaceIndex){
    if(faceIndexes[0] == oldFaceIndex){
        faceIndexes[0] = newFaceIndex;
    }
    if(faceIndexes[1] == oldFaceIndex){
        faceIndexes[1] = newFaceIndex;
    }
    if(faceIndexes[2] == oldFaceIndex){
        faceIndexes[2] = newFaceIndex;
    }
}

int Face::getVerticePosition(int verticeIndex){
    int verticePosition = -1;
    if(verticeIndexes[0] == verticeIndex){
        verticePosition = 0;
    }
    if(verticeIndexes[1] == verticeIndex){
        verticePosition = 1;
    }
    if(verticeIndexes[2] == verticeIndex){
        verticePosition = 2;
    }
    return verticePosition;
}
int Face::getFacePosition(int faceIndex){
    int facePosition = -1;
    if(faceIndexes[0] == faceIndex){
        facePosition = 0;
    }
    if(faceIndexes[1] == faceIndex){
        facePosition = 1;
    }
    if(faceIndexes[2] == faceIndex){
        facePosition = 2;
    }
    return facePosition;
}

bool Face::isInfinite(){
    return verticeIndexes[0] == 0 || verticeIndexes[1] == 0 || verticeIndexes[2] == 0;
}

void Face::print(){
    int* verticeIndex = verticeIndexes;
    int* connectedIndex = faceIndexes;
    cout << "vertice " << *(verticeIndex) << " connected to " << *(connectedIndex) << endl; 
    cout << "vertice " << *(verticeIndex + 1) << " connected to " << *(connectedIndex + 1) << endl; 
    cout << "vertice " << *(verticeIndex + 2) << " connected to " << *(connectedIndex + 2) << endl; 
}