#ifndef COORDINATES2D_H
#define COORDINATES2D_H

struct Coordinates2D {
    Coordinates2D(double lon, double lat) : lon(lon), lat(lat) {}
    double lon;
    double lat;
};

#endif // COORDINATES2D_H
