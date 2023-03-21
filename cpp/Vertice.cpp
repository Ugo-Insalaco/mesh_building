
#include "../h/Vertice.h"
#include <math.h>
using namespace std;
Vertice::Vertice(float xi, float yi, float zi): x(xi), y(yi), z(zi){};
float Vertice::normalize(){
    float sqr = sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
    if(sqr >0){
        this->x = this->x/sqr;
        this->y = this->y/sqr;
        this->z = this->z/sqr;
    }
    return sqr;
}

Vertice operator+(const Vertice& a, const Vertice& b) {
	return Vertice(a.x + b.x, a.y + b.y, a.z + b.z);
}
Vertice operator-(const Vertice& a, const Vertice& b) {
	return Vertice(a.x - b.x, a.y - b.y, a.z - b.z);
}

void Vertice::print(){
    cout << "=== vertice ===" << endl;
    cout << x << " " << y << " " << z<< endl;
}