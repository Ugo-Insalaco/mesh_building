#ifndef FACE_H
#define FACE_H

class Face{
    public:
        Face();
        Face(int index1, int index2, int index3);
        void updateVertices(int index1, int index2, int index3);
        void print();
        void connectFace(int verticeIndex, int faceIndex);
        void updateFaceIndex(int oldFaceIndex, int newFaceIndex);
        int getVerticePosition(int verticeIndex);
        bool isInfinite();
        int verticeIndexes[3];
        int faceIndexes[3];
};
#endif