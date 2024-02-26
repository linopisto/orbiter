#include "raylib.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include "Mass.h"

using namespace std;


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
    double speed_multiplier=0.05;

    int to_add_mass=10;

    masses.push_back(Mass(10, Point{300,100}, Vec{5,0}));
    masses.push_back(Mass(10000, Point{screenWidth/2,screenHeight/2}, Vec{0,0}));
    //masses.push_back(Mass(5, Point{190,190}, Vec{5,0}));

    InitWindow(screenWidth, screenHeight, "Orbiter");

    const float zoomIncrement = 0.125f;
    Camera2D camera = { 0 };
    camera.zoom=1.0f;

    SetTargetFPS(60);

    Point start_point, end_point;
    bool pause=false;

    while (!WindowShouldClose()) {

        float wheel = GetMouseWheelMove();
        if (wheel != 0){
            //camera.target = (Vector2){ player.x + 20.0f, player.y + 20.0f };
            //camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
            camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.rotation = 0.0f;
            camera.zoom += (wheel*zoomIncrement);
            if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
        }


        //listent to spacebar to pause the simulation
        if (IsKeyPressed(KEY_SPACE)) {
            pause=(!pause);
        }
        //delete all the current masses
        if (IsKeyPressed(KEY_DELETE)) {
            masses.clear();
        }
        //add/remove mass to the next added mass
        if (IsKeyDown(KEY_UP)) {
            to_add_mass+=10;
        }
        if (IsKeyDown(KEY_DOWN)) {
            to_add_mass-=10;
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

        BeginDrawing();
            ClearBackground(GRAY);
            BeginMode2D(camera);
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
            EndMode2D();

            // Set title with FPS information
            char title[64];
            sprintf(title, "FPS: %02d", GetFPS());
            SetWindowTitle(title);

        EndDrawing();
    
        
    }

    CloseWindow();

    return 0;
}