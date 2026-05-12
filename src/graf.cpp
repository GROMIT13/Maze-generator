#include "graf.hpp"

Graf::Graf()
{
}

void Graf::createVertices(int ile)
{
	vertexList.clear();
	wasNodeVisited.clear();
	vertexList.resize(ile);
	wasNodeVisited.resize(ile,false);
}

void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2)
{
	if (!isVertexValid(i_Vertex_Index_1) || !isVertexValid(i_Vertex_Index_2))
		return;

	if (checkEdge(i_Vertex_Index_1, i_Vertex_Index_2))
		return;

	vertexList[i_Vertex_Index_1].emplace_back(edge(i_Vertex_Index_1, i_Vertex_Index_2));
	vertexList[i_Vertex_Index_2].emplace_back(edge(i_Vertex_Index_2, i_Vertex_Index_1));
}

bool Graf::removeEdge(int i_Vertex_Index_1, int i_Vertex_Index_2)
{
	if (!isVertexValid(i_Vertex_Index_1) || !isVertexValid(i_Vertex_Index_2))
		return false;

	for (int i = 0; i < vertexList[i_Vertex_Index_1].size(); i++)
	{
		if (vertexList[i_Vertex_Index_1][i].vertex_Index1 == i_Vertex_Index_1 &&
			vertexList[i_Vertex_Index_1][i].vertex_Index2 == i_Vertex_Index_2)
		{
			vertexList[i_Vertex_Index_1].erase(vertexList[i_Vertex_Index_1].begin() + i);
			break;
		}
	}

	for (int i = 0; i < vertexList[i_Vertex_Index_2].size(); i++)
	{
		if (vertexList[i_Vertex_Index_2][i].vertex_Index2 == i_Vertex_Index_1 &&
			vertexList[i_Vertex_Index_2][i].vertex_Index1 == i_Vertex_Index_2)
		{
			vertexList[i_Vertex_Index_2].erase(vertexList[i_Vertex_Index_2].begin() + i);
			return true;
		}
	}

	return false;
}

bool Graf::checkEdge(int i_Vertex_Index_1, int i_Vertex_Index_2)
{
	if (!isVertexValid(i_Vertex_Index_1) || !isVertexValid(i_Vertex_Index_2))
		return false;

	for (int i = 0; i < vertexList[i_Vertex_Index_1].size(); i++)
	{
		if (vertexList[i_Vertex_Index_1][i].vertex_Index1 == i_Vertex_Index_1 &&
			vertexList[i_Vertex_Index_1][i].vertex_Index2 == i_Vertex_Index_2)
			return true;
	}

	return false;
}

int Graf::vertexDegree(int idx)
{
	if (!isVertexValid(idx))
		return 0;

	return vertexList[idx].size();
}

std::vector<int> Graf::getNeighbourIndices(int idx)
{
	std::vector<int> neighbours;

	if (!isVertexValid(idx))
		return neighbours;

	neighbours.reserve(vertexList[idx].size());
	for (int i = 0; i < vertexList[idx].size(); i++)
	{
		neighbours.push_back(vertexList[idx][i].vertex_Index2);
	}

	return neighbours;
}

void Graf::printNeighbourIndices(int idx)
{
	std::vector<int> indices = getNeighbourIndices(idx);
	for (int indice : indices)
	{
		std::cout << indice << " ";
	}
	std::cout << std::endl;
}

int Graf::getNumberOfEdges()
{
	int edges = 0;
	for (int i = 0; i < vertexList.size(); i++)
		edges += vertexList[i].size();

	return edges/2;
}

void Graf::readFromFile(std::string path)
{
	std::ifstream file(path);
	std::string vertices;
	std::string first, second;

	if (!file.is_open())
	{
		std::cout << "file " << path << " not found!" << std::endl;
		return;
	}


	std::getline(file, vertices);

	createVertices(std::stoi(vertices));

	while (file >> first >> second)
		addEdge(std::stoi(first), std::stoi(second));

}

bool Graf::isVertexValid(int idx)
{
	return (idx >= 0 && idx < vertexList.size());
}

void Graf::MarkNodeAsVisited(int idx)
{
	if(!isVertexValid(idx))
		return;

	wasNodeVisited[idx] = true;
	
}

bool Graf::WasNodeVisited(int idx)
{
	if(!isVertexValid(idx))
		return false;

    return wasNodeVisited[idx];
}
