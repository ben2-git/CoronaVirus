#ifndef UNTITLED_GRAPH_H
#define UNTITLED_GRAPH_H
#include <vector>
using namespace std;

class Graph{

public:
    Graph();
    Graph(std::vector<std::vector<int>> matrix);
    Graph(const Graph& g2);
    Graph& operator = (const Graph& g2);
    ~Graph();
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    vector<int> getNeighbors(int nodeInd) const;
    vector<vector<int>> getEdges() const;
    void removeEdges(int node);
    int size() const;

private:
    std::vector<std::vector<int>> edges;
    std::vector<bool> infected;
};

#endif //UNTITLED_GRAPH_H
