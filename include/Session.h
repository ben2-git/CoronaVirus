#ifndef UNTITLED_SESSION_H
#define UNTITLED_SESSION_H
#include <vector>
#include <string>
#include "Graph.h"
#include "queue"
#include "Tree.h"

class Agent;

enum TreeType{
    Cycle,
    MaxRank,
    Root
};

class Session{

public:
    Session(const std::string& path);
    Session(const Session& copy);
    Session& operator = (const Session& sess);
    ~ Session();
    Session(Session&& other);
    Session& operator = (Session&& other);
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    Graph getGraph () const;
    void enqueueInfected(int node);
    int dequeueInfected();
    TreeType getTreeType() const;
    Tree* BFS (int node) const;
    int getCycle () const;
    bool isVirusFree(int node) const;
    void occupyNode(int node);
    void removeEdges (int node);
    void clear();

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    queue<int> infectedQueue;
    int cycle;
    vector<bool> virusFree;
    bool finishingConditions();
    void output();
};
#endif //UNTITLED_SESSION_H
