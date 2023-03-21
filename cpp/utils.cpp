#include <math.h>
using namespace std;

Vertice cross(Vertice v1, Vertice v2, bool ignoreZ = false){
    if(ignoreZ){
        return Vertice(0,0, v1.x*v2.y - v1.y*v2.x);
    }
    return Vertice(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}
float dot(Vertice v1, Vertice v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float computeCot(Vertice v1, Vertice v2){
    v1.normalize();
    v2.normalize();
    Vertice crossProduct = cross(v1, v2);
    float s = crossProduct.normalize();
    float c = dot(v1, v2);
    return abs(c/s);
}

vector<float> computeLaplacianMesh(Mesh mesh){
    vector<float> laplacianList;
    vector<Vertice> verticeList = mesh.verticeList;
    int verticeListSize = verticeList.size();
    for(int i = 0; i< verticeListSize; i++){
        Vertice laplacian = mesh.computeLaplacian(i);
        laplacianList.push_back(laplacian.normalize());
    }
    return laplacianList;
}

Mesh getTetrahedre(){
    vector<Vertice> verticeList;
    vector<Face> faceList;

    Vertice v0(0, 0, 0);
    Vertice v1(1, 0, 0);
    Vertice v2(0, 1, 0);
    Vertice v3(0, 0, 1);

    verticeList.push_back(v0);
    verticeList.push_back(v1);
    verticeList.push_back(v2);
    verticeList.push_back(v3);

    Face f0(0, 1, 2);
    Face f1(0, 3, 1);
    Face f2(0, 2, 3);
    Face f3(1, 3, 2);

    f0.connectFace(0, 3);
    f0.connectFace(1, 1);
    f0.connectFace(2, 2);

    f1.connectFace(0, 3);
    f1.connectFace(1, 2);
    f1.connectFace(3, 0);

    f2.connectFace(0, 3);
    f2.connectFace(2, 1);
    f2.connectFace(3, 0);

    f3.connectFace(1, 2);
    f3.connectFace(2, 1);
    f3.connectFace(3, 0);

    faceList.push_back(f0);
    faceList.push_back(f1);
    faceList.push_back(f2);
    faceList.push_back(f3);

    return Mesh(verticeList, faceList);
}

void connectFacesTetrahedre(Face& f0, Face& f1, Face& f2, Face& f3, int fi0, int fi1, int fi2, int fi3){
    f0.connectFace(0, fi3);
    f0.connectFace(1, fi2);
    f0.connectFace(2, fi1);

    f1.connectFace(0, fi3);
    f1.connectFace(1, fi0);
    f1.connectFace(2, fi2);

    f2.connectFace(0, fi3);
    f2.connectFace(1, fi1);
    f2.connectFace(2, fi0);

    f3.connectFace(0, fi2);
    f3.connectFace(1, fi0);
    f3.connectFace(2, fi1);
}
void getCommonEdge(Face face1, Face face2, int& faceIndex1, int& faceIndex2, bool& invert){
    faceIndex1 = -1;
    faceIndex2 = -1;
    for(int i=0; i< 3; i++){
        for(int j=0; j<3; j++){
            // /cout << face1.verticeIndexes[i] << " " << face2.verticeIndexes[j] << " " << face1.verticeIndexes[(i+1)%3] << " " << face2.verticeIndexes[(j+2)%3] << endl;
            bool a = (face1.verticeIndexes[i] == face2.verticeIndexes[i]) && (face1.verticeIndexes[(i+1)%3] == face2.verticeIndexes[(j+2)%3]);
            // cout << a << endl;
            // cout << face1.verticeIndexes[i] << " " << face2.verticeIndexes[j] << " " << face1.verticeIndexes[(i+2)%3] << " " << face2.verticeIndexes[(j+1)%3] << endl;
            bool b = (face1.verticeIndexes[i] == face2.verticeIndexes[i]) && (face1.verticeIndexes[(i+2)%3] == face2.verticeIndexes[(j+1)%3]);
            // cout << b << endl;
            if((face1.verticeIndexes[i] == face2.verticeIndexes[j]) && (face1.verticeIndexes[(i+1)%3] == face2.verticeIndexes[(j+2)%3])){
                faceIndex1 = i;
                faceIndex2 = j;
                invert = false;
                return;
            }
            if((face1.verticeIndexes[i] == face2.verticeIndexes[j]) && (face1.verticeIndexes[(i+2)%3] == face2.verticeIndexes[(j+1)%3])){
                faceIndex1 = i;
                faceIndex2 = j;
                invert = true;
                return;
            }
        }
    }
}

double orientationTest(Vertice vertice1, Vertice vertice2, Vertice vertice3){
    // On ignore la coordonnées z
    return (-1)*dot(cross(vertice2 - vertice1, vertice3 - vertice1, true), Vertice(0,0,1));
}

double verticeInTriangle(Vertice vertice, Vertice triangleVertice1, Vertice triangleVertice2, Vertice triangleVertice3){
    // On ignore encore la coordonnée z
    double pred1 = orientationTest(vertice, triangleVertice1, triangleVertice2);
    double pred2 = orientationTest(vertice, triangleVertice2, triangleVertice3);
    double pred3 = orientationTest(vertice, triangleVertice3, triangleVertice1);

    if(pred1 == 0 || pred2 == 0 || pred3 == 0){
        return 0;
    }
    if(pred1 > 0 || pred2 > 0 || pred3 > 0){
        return -1;
    }
    return 1;
}

Vertice phi(Vertice vertice){
    return Vertice(vertice.x, vertice.y, vertice.x * vertice.x + vertice.y * vertice.y);
}

double isInCircle(Vertice v1, Vertice v2, Vertice v3, Vertice v4){
    // On teste que v1 est dans le cercle des trois autres
    return (-1)*dot(cross(phi(v3 - v2), phi(v4 - v2)), phi(v1 - v2));
}