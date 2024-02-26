#include "Mass.h"
#include <math.h>
#include <vector>

Mass::Mass(int m, Point p, Vec v){
    position = p;
    velocity = v;
    mass = m;
    id = counter;
    counter++;
}

void Mass::update_position(){
    position.x = position.x + cos(velocity.t) * velocity.r;
    position.y = position.y + sin(velocity.t) * velocity.r;
}

void Mass::update_velocity(std::vector<Mass>& ms){
    for(int i = 0; i<ms.size();i++){
        if(ms[i].id != this->id){
            double dx = ms[i].position.x - position.x;
            double dy = ms[i].position.y - position.y;
            double d = sqrt(pow(dx,2)+pow(dy,2));               //distance between centers
            double f = K*(this->mass * ms[i].mass) / pow(d,2);  //force modulo
            Vec f_=Vec{f, atan2(dy,dx)};                        //force vector
            Vec a_=Vec{f_.r/this->mass, f_.t};                  //acceleration vector
            this->velocity=cart2polar(polar2cart(this->velocity)+polar2cart(a_));
        }
    }
}

void Mass::draw(){
    // draw the object body
    DrawCircle(position.x, position.y, log(log(mass))*size, color);
    int len_multiplier=20;
    //show the direction of the speed vector
    DrawLine(position.x, position.y, 
            position.x+velocity.r*cos(velocity.t)*len_multiplier, 
            position.y+velocity.r*sin(velocity.t)*len_multiplier,
            WHITE);
}

int Mass::counter=0;



Point operator+(const Point& p1, const Point& p2){
    return Point{p1.x+p2.x, p1.y+p2.y}; 
}
Point operator-(const Point& p1, const Point& p2){
    return Point{p1.x-p2.x, p1.y-p2.y}; 
}

float distance(Point a, Point b){
    return(sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)));
}

//vector representaiton conversion
CVec polar2cart(Vec v){
    return CVec{v.r*cos(v.t), v.r*sin(v.t) };
}
Vec cart2polar(CVec v){
    return Vec{sqrt(pow(v.x, 2)+pow(v.y, 2)),
                atan2(v.y,v.x)};
}

//basic vector operation
CVec operator+(const CVec& vec1, const CVec& vec2) {
    CVec result;
    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    return result;
}
CVec operator-(const CVec& vec1, const CVec& vec2) {
    CVec result;
    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    return result;
}
Vec operator+(const Vec& vec1, const Vec& vec2){
    return cart2polar(polar2cart(vec1) + polar2cart(vec2));
}
Vec operator-(const Vec& vec1, const Vec& vec2){
    return cart2polar(polar2cart(vec1) - polar2cart(vec2));
}

//displace a point by a vector
Point displace(Point p, Vec v){
    return Point{p.x+v.r*cos(v.t),p.y+v.r*sin(v.t)};
}