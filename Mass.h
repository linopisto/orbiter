#ifndef MASS_H
#define MASS_H

#include "raylib.h"
#include <vector>
#include <math.h>

#define K 0.5   //gravity constant

struct Vec{
    double r,t;
};

struct CVec{
    double x,y;
};

struct Point{
    double x,y;
};

class Mass{
    public: 
    double mass=0;
    int size=10;
    int id;
    static int counter;
    Vec velocity;
    Point position;
    Color color=BLACK;

    Mass(int m, Point p, Vec v);

    void update_position();

    void update_velocity(std::vector<Mass>& ms);

    void draw();
};

Point operator+(const Point& p1, const Point& p2);

Point operator-(const Point& p1, const Point& p2);

float distance(Point a, Point b);

CVec polar2cart(Vec v);

Vec cart2polar(CVec v);

CVec operator+(const CVec& vec1, const CVec& vec2);

CVec operator-(const CVec& vec1, const CVec& vec2);

Vec operator+(const Vec& vec1, const Vec& vec2);

Vec operator-(const Vec& vec1, const Vec& vec2);

Point displace(Point p, Vec v);
#endif // MASS_H
