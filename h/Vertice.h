#ifndef VERTICE_H
#define VERTICE_H

class Vertice{
    public:
        Vertice(float xi, float yi, float zi);
        float normalize();
        void print();
        float x;
        float y;
        float z;
        int faceIndex;
};

#endif