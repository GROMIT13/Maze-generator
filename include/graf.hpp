#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "edge.hpp"


class Graf
{
    std::vector<std::vector<edge>> vertexList;
    std::vector<bool> wasNodeVisited;
    std::vector<bool> wasNodeBacktracked;

public:
    Graf();
    void createVertices(int ile);    
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);    
    bool removeEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    bool checkEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    int vertexDegree(int idx); 
    std::vector<int> getNeighbourIndices(int idx);
    void printNeighbourIndices(int idx);
    int getNumberOfEdges();
    void readFromFile(std::string path);
    bool isVertexValid(int idx);
    void MarkNodeAsVisited(int idx);
    bool WasNodeVisited(int idx);
    void MarkNodeAsBacktracked(int idx);
    bool WasNodeBacktracked(int idx);
};

