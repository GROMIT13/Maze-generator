#include "raylib.h"
#include <stdlib.h>
#include "maze.hpp"
#include "graf.hpp"

int main(int argn, char** argv)
{
    srand(time(NULL));

    int width;
    int height;

    if(argn == 1)
    {
        width = 10;
        height = 10;
    }

    if(argn == 3)
    {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight,TextFormat("Maze generator [ %d x %d ]", width, height));

    Maze maze(width, height);
    maze.GenerateAnimatedInit(0,0);
    float generateTime = 0.0;
    float frameTime = 10.0/(width * height);
    
    SetTargetFPS(60); 
    
    while (!WindowShouldClose())
    {
        // Update
        maze.SetCellSizeAndMazePosition();
        generateTime += GetFrameTime();
        while (generateTime > frameTime)
        {
            generateTime -= frameTime;
            maze.GenerateAnimated();
        }
        

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            maze.Draw();

        EndDrawing();

    }

    // De-Initialization
    CloseWindow();

}
