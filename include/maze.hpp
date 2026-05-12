#pragma once

#include <stack>
#include "raylib.h"
#include "graf.hpp"
#include "vec2.hpp"

//WARNING: initialize this class after raylib "InitWindow()"
class Maze
{
private:
    const int width;
    const int height;
    float cellSize;
    float lineWidth;
    float marginSize; //Size of the margin from maze 
    Vector2 gridPos;
    Graf graph;

public:
    Maze(int width, int height);
    ~Maze();
    void Draw();
    void SetCellSizeAndMazePosition(); //calculate grid size to cover given percent of the screen height

private:
    int PosToGraphIndex(int x, int y);
    Vec2 GraphIndexToPos(int index);
    bool isPosValid(int x, int y);
    void AddEdgeNorth(int x, int y);
    void AddEdgeSouth(int x, int y);
    void AddEdgeEast(int x, int y);
    void AddEdgeWest(int x, int y);
    bool CheckEdgeNorth(int x, int y);
    bool CheckEdgeSouth(int x, int y);
    bool CheckEdgeEast(int x, int y);
    bool CheckEdgeWest(int x, int y);
    //Contert Cell pos to screen position. Returns left upper position of a cell
    Vector2 GetCellScreenCoordinates(float x, float y);
    void MarkNodeAsVisited(int x, int y);
};

Maze::Maze(int width, int height)
    :width(width), height(height), lineWidth(3), marginSize(0.05)
{
    SetCellSizeAndMazePosition();
    graph.createVertices(width * height);
    AddEdgeEast(2,3);
    AddEdgeWest(2,3);
    AddEdgeNorth(2,3);
    AddEdgeSouth(2,3);
    MarkNodeAsVisited(6,7);
}

Maze::~Maze()
{
}

void Maze::Draw()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(!CheckEdgeWest(x,y))
                DrawLineEx(GetCellScreenCoordinates(x,y),GetCellScreenCoordinates(x,y-1),lineWidth, DARKGRAY);
            if(!CheckEdgeNorth(x,y))
                DrawLineEx(GetCellScreenCoordinates(x,y),GetCellScreenCoordinates(x+1,y),lineWidth, DARKGRAY);
            if(graph.WasNodeVisited(PosToGraphIndex(x,y)))
                DrawRectangle(GetCellScreenCoordinates(x+0.25,y-0.25).x,GetCellScreenCoordinates(x+0.25,y-0.25).y,cellSize/2.0,cellSize/2.0,RED);
            
        }
    }
    //Draw bottom an right border
    DrawLineEx(GetCellScreenCoordinates(0,-1),GetCellScreenCoordinates(width,-1),lineWidth, DARKGRAY);
    DrawLineEx(GetCellScreenCoordinates(width,-1),GetCellScreenCoordinates(width,height-1),lineWidth, DARKGRAY);
    
}

int Maze::PosToGraphIndex(int x, int y)
{
    return y * width + x;
}

Vec2 Maze::GraphIndexToPos(int index)
{
    return Vec2 {index % width, (index - (index % width))/width};
}

bool Maze::isPosValid(int x, int y)
{
    return (x < width && x >= 0) && (y < height && y >= 0);
}

void Maze::AddEdgeNorth(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x,y+1))
        return;

    graph.addEdge(PosToGraphIndex(x,y),PosToGraphIndex(x,y+1));
}

void Maze::AddEdgeSouth(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x,y-1))
        return;

    graph.addEdge(PosToGraphIndex(x,y),PosToGraphIndex(x,y-1));
}

void Maze::AddEdgeEast(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x+1,y))
        return;

    graph.addEdge(PosToGraphIndex(x,y),PosToGraphIndex(x+1,y));
}

void Maze::AddEdgeWest(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x-1,y))
        return;

    graph.addEdge(PosToGraphIndex(x,y),PosToGraphIndex(x-1,y));
}

bool Maze::CheckEdgeNorth(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x,y+1))
        return false;

    return graph.checkEdge(PosToGraphIndex(x,y),PosToGraphIndex(x,y+1));
}

bool Maze::CheckEdgeSouth(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x,y-1))
        return false;

    return graph.checkEdge(PosToGraphIndex(x,y),PosToGraphIndex(x,y-1));
}

bool Maze::CheckEdgeEast(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x+1,y))
        return false;

    return graph.checkEdge(PosToGraphIndex(x,y),PosToGraphIndex(x+1,y));
}

bool Maze::CheckEdgeWest(int x, int y)
{
    if(!isPosValid(x,y) || !isPosValid(x-1,y))
        return false;

    return graph.checkEdge(PosToGraphIndex(x,y),PosToGraphIndex(x-1,y));
}

Vector2 Maze::GetCellScreenCoordinates(float x, float y)
{   
    return Vector2{x * cellSize + gridPos.x,-y * cellSize + cellSize*height-cellSize + gridPos.y};
}

void Maze::MarkNodeAsVisited(int x, int y)
{
    if(!isPosValid(x,y))
        return;

    graph.MarkNodeAsVisited(PosToGraphIndex(x,y));
}

void Maze::SetCellSizeAndMazePosition()
{
    float screenHeigt = (float)GetScreenHeight();
    float screenWidth = (float)GetScreenWidth();
    float screenRatio = screenHeigt/screenWidth;
    float cellRatio = (float)height/width;
    std::cout << screenRatio << ", " << cellRatio << "\n";
    if(cellRatio > screenRatio)
    {
        gridPos.x = screenWidth/2.0 - cellSize * width / 2.0;
        gridPos.y = screenHeigt * marginSize;
        cellSize = screenHeigt * (1.0-marginSize*2) / (float)height;
    }
    else
    {
        cellSize = screenWidth * (1.0-marginSize*2) / (float)width;
        gridPos.x = screenWidth * marginSize;
        gridPos.y = screenHeigt/2.0 - cellSize * height / 2.0;

    }
}
