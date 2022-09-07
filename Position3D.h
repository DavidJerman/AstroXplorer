#ifndef POSITION3D_H
#define POSITION3D_H

struct Position3D {
    Position3D(double x, double y, double z) : x(x), y(y), z(z) {};
    double x;
    double y;
    double z;
};

#endif // POSITION3D_H
