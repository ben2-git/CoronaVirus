#include "../include/Agent.h"
#include <iostream>
#include "../include/Session.h"
using namespace std;

Agent::Agent() {};
Agent::~Agent() {};
ContactTracer::ContactTracer(){}
ContactTracer::~ContactTracer(){};
Virus::Virus(int _nodeInd): nodeInd(_nodeInd) {};
Virus::~Virus() {};

Agent* ContactTracer::clone() const {
    return new ContactTracer();
}

void ContactTracer::act(Session &session) {
    int curr = session.dequeueInfected();
    if(curr != -1) {//  Q not empty
        Tree *tmp = session.BFS(curr);//create tree
        int nodeToRemove = tmp->traceTree(); //using the tree's trace tree method to discover the node we need to isolate
        delete tmp;
        session.removeEdges(nodeToRemove);//isolate the node
    }
}

Agent * Virus::clone() const {
    return new Virus(nodeInd);
}

void Virus::act(Session &session) {
    if(!session.getGraph().isInfected(nodeInd)){
        //we have not yet infected this node
        session.enqueueInfected(nodeInd);
    }
    //spreading to the smallest indexed virus-free neighbor
    vector<int> neighbors = session.getGraph().getNeighbors(nodeInd);
    bool spread = false;
    int size = neighbors.size();
    for(int i=0; (i < size) & (!spread); i++){
        if(session.isVirusFree(neighbors[i]))
        {
            spread = true;
            session.occupyNode(neighbors[i]);
            session.addAgent(Virus(neighbors[i]));
        }
    }

};
