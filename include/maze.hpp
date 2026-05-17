#pragma once

#include <stack>
#include <cmath>
#include "raylib.h"
#include "graf.hpp"
#include "vec2.hpp"

enum class Direction 
{
    NORTH = 0,
    SOUTH,
    EAST,
    WEST
};

struct MazeGenerationState
{
    bool hasGnerationFinished;
    std::stack<Vec2> cellStack;
    Vec2 currentPos;
};


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
    MazeGenerationState mazeGenerationState;

public:
    Maze(int width, int height);
    ~Maze();
    void Draw();
    // Generte maze starting with given cell position
    void Generate(int x, int y);
    void GenerateAnimatedInit(int x, int y);
    void GenerateAnimated();
     //calculate grid size to cover given percent of the screen height
    void SetCellSizeAndMazePosition();

private:
    int PosToGraphIndex(int x, int y);
    Vec2 GraphIndexToPos(int index);
    bool isPosValid(int x, int y);
    void AddEdgeNorth(int x, int y);
    void AddEdgeSouth(int x, int y);
    void AddEdgeEast(int x, int y);
    void AddEdgeWest(int x, int y);
    void AddEdge(int x, int y, Direction direction);
    Vec2 MoveDirection(Vec2 vec2, Direction);
    bool CheckEdgeNorth(int x, int y);
    bool CheckEdgeSouth(int x, int y);
    bool CheckEdgeEast(int x, int y);
    bool CheckEdgeWest(int x, int y);
    // Contert Cell pos to screen position. Returns left upper position of a cell
    Vector2 GetCellScreenCoordinates(float x, float y);
    void MarkNodeAsVisited(int x, int y);
    bool WasNodeVisited(int x, int y);
    void MarkNodeAsBacktracked(int x, int y);
    bool WasNodeBacktracked(int x, int y);
    std::vector<Direction> GetValidDirections(int x, int y);
    Direction GetRandomDirection(std::vector<Direction> directions);
};

Maze::Maze(int width, int height)
    :width(width), height(height), lineWidth(4), marginSize(0.05)
{
    SetCellSizeAndMazePosition();
    graph.createVertices(width * height);
}

Maze::~Maze()
{
}

void Maze::Draw()
{
    //Draw Lines
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(!CheckEdgeWest(x,y))
                DrawLineEx(GetCellScreenCoordinates(x,y),GetCellScreenCoordinates(x,y-1),lineWidth, DARKGRAY);
            if(!CheckEdgeNorth(x,y))
                DrawLineEx(GetCellScreenCoordinates(x,y),GetCellScreenCoordinates(x+1,y),lineWidth, DARKGRAY);
        }
    }
    //Draw bottom an right border
    DrawLineEx(GetCellScreenCoordinates(0,-1),GetCellScreenCoordinates(width,-1),lineWidth, DARKGRAY);
    DrawLineEx(GetCellScreenCoordinates(width,-1),GetCellScreenCoordinates(width,height-1),lineWidth, DARKGRAY);

    //Draw connections
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(WasNodeVisited(x,y) && CheckEdgeSouth(x,y))
                DrawRectangle(GetCellScreenCoordinates(x+0.25,y-0.25).x,GetCellScreenCoordinates(x+0.25,y-0.25).y,cellSize/2.0,3 * floor(cellSize/2.0),RED);
            if(WasNodeVisited(x,y) && CheckEdgeEast(x,y))
               DrawRectangle(GetCellScreenCoordinates(x+0.25,y-0.25).x,GetCellScreenCoordinates(x+0.25,y-0.25).y,3 * floor(cellSize/2.0),cellSize/2.0,RED);
        }
    }
    //Draw backtracked
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(WasNodeBacktracked(x,y) && CheckEdgeSouth(x,y) && WasNodeBacktracked(x,y-1))
                DrawRectangle(GetCellScreenCoordinates(x+0.25,y-0.25).x,GetCellScreenCoordinates(x+0.25,y-0.25).y,cellSize/2.0,3 * floor(cellSize/2.0),BLUE);
            if(WasNodeBacktracked(x,y) && CheckEdgeEast(x,y) && WasNodeBacktracked(x+1,y))
               DrawRectangle(GetCellScreenCoordinates(x+0.25,y-0.25).x,GetCellScreenCoordinates(x+0.25,y-0.25).y,3 * floor(cellSize/2.0),cellSize/2.0,BLUE);
            if(WasNodeBacktracked(x,y))
                DrawRectangle(GetCellScreenCoordinates(x+0.25,y-0.25).x,GetCellScreenCoordinates(x+0.25,y-0.25).y,cellSize/2.0,cellSize/2.0,BLUE);
        }
    }
    
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

void Maze::AddEdge(int x, int y, Direction direction)
{
    if(direction == Direction::NORTH)
        AddEdgeNorth(x,y);
    if(direction == Direction::SOUTH)
        AddEdgeSouth(x,y);
    if(direction == Direction::EAST)
        AddEdgeEast(x,y);
    if(direction == Direction::WEST)
        AddEdgeWest(x,y);
}

Vec2 Maze::MoveDirection(Vec2 vec2, Direction direction)
{
    if(direction == Direction::NORTH)
        return Vec2{vec2.x, vec2.y+1};
    if(direction == Direction::SOUTH)
        return Vec2{vec2.x, vec2.y-1};
    if(direction == Direction::EAST)
        return Vec2{vec2.x+1, vec2.y};
    if(direction == Direction::WEST)
        return Vec2{vec2.x-1, vec2.y};

    return Vec2{-1,-1};
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

bool Maze::WasNodeVisited(int x, int y)
{
    if(!isPosValid(x,y))
        return false;

    return graph.WasNodeVisited(PosToGraphIndex(x,y));
}

void Maze::MarkNodeAsBacktracked(int x, int y)
{
    if(!isPosValid(x,y))
        return;

    graph.MarkNodeAsBacktracked(PosToGraphIndex(x,y));
}

bool Maze::WasNodeBacktracked(int x, int y)
{
    if(!isPosValid(x,y))
        return false;

    return graph.WasNodeBacktracked(PosToGraphIndex(x,y));
}

std::vector<Direction> Maze::GetValidDirections(int x, int y)
{
    std::vector<Direction> directions;
    directions.reserve(4);

    if(!WasNodeVisited(x,y+1) && isPosValid(x,y+1))
        directions.push_back(Direction::NORTH);
    if(!WasNodeVisited(x,y-1) && isPosValid(x,y-1))
        directions.push_back(Direction::SOUTH);
    if(!WasNodeVisited(x+1,y) && isPosValid(x+1,y))
        directions.push_back(Direction::EAST);
    if(!WasNodeVisited(x-1,y) && isPosValid(x-1,y))
        directions.push_back(Direction::WEST);

    return directions;
}

Direction Maze::GetRandomDirection(std::vector<Direction> directions)
{
    int r = rand() % directions.size();
    return directions[r];
}

void Maze::Generate(int x, int y)
{
    std::stack<Vec2> cellStack;
    cellStack.push(Vec2{x,y});

    Vec2 currentPos = Vec2{x,y};
    MarkNodeAsVisited(currentPos.x,currentPos.y);

    while (!cellStack.empty())
    {
        std::vector<Direction> validDirections = GetValidDirections(currentPos.x,currentPos.y);
        if(!validDirections.empty())
        {
            Direction randomDirection = GetRandomDirection(validDirections);
            AddEdge(currentPos.x, currentPos.y, randomDirection);
            currentPos = MoveDirection(currentPos,randomDirection);
            MarkNodeAsVisited(currentPos.x,currentPos.y);
            cellStack.push(currentPos);
        }
        else
        {
            currentPos = cellStack.top();
            cellStack.pop();
        }
    }

}

void Maze::GenerateAnimatedInit(int x, int y)
{
    mazeGenerationState.hasGnerationFinished = false;
    mazeGenerationState.currentPos = Vec2{x,y};
    mazeGenerationState.cellStack.push(Vec2{x,y});
    MarkNodeAsVisited(x,y);
}

void Maze::GenerateAnimated()
{
    if (!mazeGenerationState.cellStack.empty())
    {
        std::vector<Direction> validDirections = GetValidDirections(mazeGenerationState.currentPos.x,mazeGenerationState.currentPos.y);
        if(!validDirections.empty())
        {
            Direction randomDirection = GetRandomDirection(validDirections);
            AddEdge(mazeGenerationState.currentPos.x, mazeGenerationState.currentPos.y, randomDirection);
            mazeGenerationState.currentPos = MoveDirection(mazeGenerationState.currentPos,randomDirection);
            MarkNodeAsVisited(mazeGenerationState.currentPos.x,mazeGenerationState.currentPos.y);
            mazeGenerationState.cellStack.push(mazeGenerationState.currentPos);
        }
        else
        {
            mazeGenerationState.currentPos = mazeGenerationState.cellStack.top();
            MarkNodeAsBacktracked(mazeGenerationState.currentPos.x, mazeGenerationState.currentPos.y);
            mazeGenerationState.cellStack.pop();
        }
    }
}

void Maze::SetCellSizeAndMazePosition()
{
    float screenHeigt = (float)GetScreenHeight();
    float screenWidth = (float)GetScreenWidth();
    float screenRatio = screenHeigt/screenWidth;
    float cellRatio = (float)height/width;
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
