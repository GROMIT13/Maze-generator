#include "raylib.h"
#include <stdlib.h>
#include "maze.hpp"
#include "graf.hpp"

int main(int argn, char** argv)
{
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");


    Maze maze(width, height);

    while (!WindowShouldClose())
    {
        // Update
        maze.SetCellSizeAndMazePosition();

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            maze.Draw();

        EndDrawing();

    }

    // De-Initialization
    CloseWindow();

}
