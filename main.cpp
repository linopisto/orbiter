#include "raylib.h"
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

struct Vec{
    float x,y;
    float r,t;
};
struct Point{
    float x,y;
};

class Mass{
    public: 
    float mass=0;
    int size=20;
    int id;
    static int counter=0;
    Vec velocity;
    Point position;

    Mass(int m, Point p, Vec v){
        position = p;
        velocity = v;
        mass = m;
        id = counter;
        counter++;
    }

    void update_position(){
        position.x = position.x + cos(vec.t) * vec.r:
        position.y = position.y + sin(vec.t) * vec.r;
    }

    void update_velocity(){
        ;
        //TODO: we must check for all the other objects in the vector and add theyr component
    }

    void draw(){
        // Draw circle
        DrawCircle(position.x, position.y, size, BLACK);
    }

};

vector<Mass> masses;

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Orbiter");

    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        // Draw circle
        for(int i=0; i<masses.size(); i++){
            masses[i].update_velocity();
            masses[i].update_position();
            masses[i].draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}