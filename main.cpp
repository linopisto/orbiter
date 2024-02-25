#include "raylib.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#define K 0.5   //gravity constant

using namespace std;

struct Vec{
    double r,t;
};
struct CVec{
    double x,y;
};
struct Point{
    double x,y;
};

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


class Mass{
    public: 
    double mass=0;
    int size=10;
    int id;
    static int counter;
    Vec velocity;
    Point position;
    Color color=BLACK;
    
    Mass(int m, Point p, Vec v){
        position = p;
        velocity = v;
        mass = m;
        id = counter;
        counter++;
    }

    void update_position(){
        position.x = position.x + cos(velocity.t) * velocity.r;
        position.y = position.y + sin(velocity.t) * velocity.r;
    }

    void update_velocity(vector<Mass>& ms){
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

    void draw(){
        // draw the object body
        DrawCircle(position.x, position.y, log(log(mass))*size, color);
        int len_multiplier=20;
        //show the direction of the speed vector
        DrawLine(position.x, position.y, 
                position.x+velocity.r*cos(velocity.t)*len_multiplier, 
                position.y+velocity.r*sin(velocity.t)*len_multiplier,
                WHITE);
    }

};
int Mass::counter=0;

void draw_arrow(Point p, Vec v, double multiplier){
    Point to=Point{p.x+v.r*cos(v.t)*multiplier,
                   p.y+v.r*sin(v.t)*multiplier};
    //TODO: it works only with angle of PI/2
    Point p1=displace(displace(to, Vec{-v.r/10,v.t}), Vec{-v.r/10,v.t+PI/2});
    Point p2=displace(displace(to, Vec{-v.r/10,v.t}), Vec{+v.r/10,v.t+PI/2});
    DrawLine(p.x, p.y, to.x, to.y, BLACK);                       // Horizontal line
    DrawLine(p1.x, p1.y, to.x, to.y, BLACK); // Top diagonal
    DrawLine(p2.x, p2.y, to.x, to.y, BLACK); // Bottom diagonal
}

vector<Mass> masses;

void build_grid(int w, int h, int cell_size, vector<Mass>& m){
    int rows=h/cell_size; 
    int cols=w/cell_size;
    vector<Mass> temp;

    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            Point center= Point{j*cell_size+cell_size/2, i*cell_size+cell_size/2};
            //DrawCircle(center.x, center.y,2,WHITE);
            //TODO: make the mass to be a float, 1 is to much as minimum
            Mass m = Mass(1,center,Vec{0,0});
            m.update_velocity(masses);
            if(m.velocity.r>0.5&&m.velocity.r<7){
                draw_arrow(center,m.velocity,10);
            }
            temp.push_back(m);
        }
    }
}
void draw_grid();

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    const int screenWidth = 800;
    const int screenHeight = 450;
    double scroll_speed=10;
    double speed_multiplier=0.05;

    int to_add_mass=10;

    masses.push_back(Mass(10, Point{300,100}, Vec{5,0}));
    masses.push_back(Mass(10000, Point{screenWidth/2,screenHeight/2}, Vec{0,0}));
    //masses.push_back(Mass(5, Point{190,190}, Vec{5,0}));

    InitWindow(screenWidth, screenHeight, "Orbiter");

    SetTargetFPS(60);

    Point start_point, end_point;
    bool pause=false;

    while (!WindowShouldClose()) {
        
        //listent to spacebar to pause the simulation
        if (IsKeyPressed(KEY_SPACE)) {
            pause=(!pause);
        }
        //delete all the current masses
        if (IsKeyPressed(KEY_DELETE)) {
            masses.clear();
        }
        //corse mass adjustment
        if (IsKeyPressed(KEY_KP_SUBTRACT)) {
            to_add_mass-=1000;
        }
        if (IsKeyPressed(KEY_KP_ADD)) {
            to_add_mass+=1000;
        }
        //if leftbutton pressed we save the location in order to calculate the speed vector
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            start_point=Point{(double)GetMouseX(),(double)GetMouseY()};
        }
        //left button reelase add a new mass with a speed vector proportional to the mouse drag  
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            end_point=Point{(double)GetMouseX(),(double)GetMouseY()};
            Point delta=end_point-start_point;
            Vec new_vel=Vec{distance(start_point,end_point)*speed_multiplier,
                            atan2(delta.y,delta.x)};
            masses.push_back(Mass(to_add_mass, start_point, new_vel));
        }
        //increase/decrease the mass for the future new objects
        to_add_mass+=GetMouseWheelMove()*scroll_speed;

        BeginDrawing();
        ClearBackground(GRAY);
        //for each object update velocity vector, position and draw
        for(int i=0; i<masses.size(); i++){
            if(!pause){
                masses[i].update_velocity(masses);
                masses[i].update_position();
            }
            masses[i].draw();
        }
        //dispay the mass for the future object
        string s="size "+to_string(to_add_mass);
        DrawText(s.c_str(),5,10,15, WHITE);

        //draw_arrow(Point{150,100}, Vec{100, -PI/2});
        build_grid(screenWidth,screenHeight, 10, masses);

        // Set title with FPS information
        char title[64];
        sprintf(title, "FPS: %02d", GetFPS());
        SetWindowTitle(title);

        EndDrawing();
    
        
    }

    CloseWindow();

    return 0;
}