#include "../include/Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(std::vector<std::vector<int>> matrix):edges(matrix),infected(matrix.size(),false){};

Graph::Graph(const Graph& g):edges(g.edges), infected(g.infected){};

Graph::Graph():edges(), infected() {}

Graph::~Graph() {}

Graph& Graph::operator=(const Graph& g2){
    if(this!=&g2) {
        edges = g2.edges;
        infected = g2.infected;
    }
    return *this;
};

void Graph::infectNode(int nodeInd){
    infected[nodeInd]=true;
};

bool Graph:: isInfected(int nodeInd){
    return infected[nodeInd];
};

vector<vector<int>> Graph::getEdges() const {
    return edges;
}

vector<int> Graph::getNeighbors(int nodeInd) const {
    vector<int> neighbors;
    int size = edges.size();
    for(int i=0; i<size; i++){
        // go over all edges connected to the vertex
        if(edges[nodeInd][i]==1){
            neighbors.push_back(i);
        }
    }
    return neighbors;
};

void Graph::removeEdges(int node) {
    // delete edges connected to the node from matrix
    int size= edges.size();
    for(int i=0; i<size; i++){
        edges[i][node]=0;
        edges[node][i]=0;
    }
}

int Graph::size() const {
    return edges.size();
}
