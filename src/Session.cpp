#include "../include/Session.h"
#include "../include/Agent.h"
#include "queue"
#include "fstream"
#include "../include/json.hpp"

Session::Session(const std::string &path):g(), treeType(), agents(), infectedQueue(),cycle(0), virusFree(){
    //reading the json file
    std::ifstream i(path);
    nlohmann::json j;
    i>>j;
    g = Graph(j["graph"]);
    virusFree = vector<bool>(g.size(), true);
    string type = j["tree"];
    if(type=="M")
        treeType = MaxRank;
    if(type=="C")
        treeType = Cycle;
    if(type=="R")
        treeType = Root;
    int size = j["agents"].size();
    for(int k =0; k<size; k++){
        Agent* nextAgent;
        if(j["agents"][k][0]=="V"){
            nextAgent = new Virus(j["agents"][k][1]);
            occupyNode(j["agents"][k][1]);
        }
        else
            nextAgent = new ContactTracer();
        agents.push_back(nextAgent);
    }
}

Session::Session(const Session &copy): g(copy.getGraph()), treeType(copy.treeType), agents(), infectedQueue(copy.infectedQueue), cycle(copy.cycle), virusFree(copy.virusFree) {
    int size = copy.agents.size();
    for(int i=0; i<size;i++){
        agents.push_back(copy.agents[i]->clone());
    }
}

Session& Session::operator=(const Session &sess) {//operator
    if(this == &sess)
        return *this;
    clear();
    int size = sess.agents.size();
    for(int i=0; i<size ;i++){
        agents.push_back(sess.agents[i]->clone());
    }
    virusFree=sess.virusFree;
    cycle=sess.cycle;
    infectedQueue=sess.infectedQueue;
    g=sess.g;
    return *this;
}

Session::Session(Session &&other): g(other.g),treeType(other.treeType),agents(), infectedQueue(other.infectedQueue), cycle(other.cycle),virusFree(other.virusFree) {
    int size = other.agents.size();
    for(int i=0; i<size ;i++){
        agents.push_back(other.agents[i]);
    }
    other.agents.clear();
}

Session& Session::operator=(Session &&other) {
    if(this != &other){
        clear();
        int size = other.agents.size();
        for(int i=0; i<size; i++){
            agents.push_back(other.agents[i]);
        }
        g=other.g;
        treeType=other.treeType;
        cycle=other.cycle;
        infectedQueue=other.infectedQueue;
        virusFree=other.virusFree;
        other.agents.clear();
    }
    return *this;
}

Session::~Session() {clear();}

void Session::clear() {
    int size = agents.size();
    for(int i=0; i<size; i++){
        delete agents[i];
    }
}

void Session::addAgent(const Agent &agent) {
    Agent* clone = agent.clone();
    agents.push_back(clone);
}

Graph Session::getGraph() const {
    return g;
}

void Session::setGraph(const Graph &graph) {
    g = Graph(graph);
}

void Session::enqueueInfected(int node) {
    infectedQueue.push(node);
    g.infectNode(node);
}

int Session::dequeueInfected() {
    if(infectedQueue.empty())
        return -1;
    int ans = infectedQueue.front();
    infectedQueue.pop();
    return ans;
}

TreeType Session::getTreeType() const {
    return treeType;
}

Tree* Session::BFS(int node) const{
    //standard BFS algorithm using a queue
    Tree* finalTree = Tree:: createTree(*this , node);
    vector<bool> check(g.size(), false);
    queue<Tree*> qBFS;
    qBFS.push(finalTree);
    check[node] = true;
    while(!qBFS.empty()){
        Tree* tmpTree = qBFS.front();
        vector<int> tmp = g.getNeighbors(qBFS.front()->getNode());
        int VecSize= tmp.size();
        qBFS.pop();
        for(int i = 0; i < VecSize; i++){
            if(!check[tmp[i]]) {
                Tree *addTree = Tree::createTree(*this, tmp[i]);
                qBFS.push(addTree);
                check[tmp[i]] = true;
                tmpTree->addChild(addTree);
            }
        }
    }
    return finalTree;
}

int Session::getCycle() const {return cycle;}

bool Session::isVirusFree(int node) const {return virusFree[node];}

void Session::occupyNode(int node) {virusFree[node] = false;}

void Session::removeEdges(int node) {g.removeEdges(node);}

void Session::simulate() {
    bool finishCon = false;
    while(!finishCon){ //the session continues until the finishing conditions are met
        int agentsNum = agents.size();
        for(int i=0; i<agentsNum; i++)
            agents[i]->act(*this); //each agent is acting by order
        cycle++;
        finishCon = finishingConditions(); //checking to see if we have reached finishing conditions
    }
    //outputting the json file
    output();
}

bool Session::finishingConditions(){
    //checking each graph compoment, to see if its infected entirely or does not contain any virus
    queue<int> bfsQ; //using queue for BFS to check graph components
    vector<bool> checked(g.size(), false);
    int size = checked.size();
    for(int i=0; i<size; i++){
        if(!checked[i]) {//'checked' means we checked this node in his component
            //we havent checked the node 'i' connected component yet
            checked[i] = true;
            bool virusFree = isVirusFree(i); //if i has a virus, we will check if all nodes in the component are infected
            //if i does not, we will check if any node in the component is a virus
            bfsQ.push(i);
            while(!bfsQ.empty()){
                int node = bfsQ.front();
                bfsQ.pop();
                if((!virusFree) & (virusFree == g.isInfected(node))) {
                    //the node 'i' was a virus, but this 'node' is not infected, and they are in the same component
                    return false;
                }
                if((virusFree) & (!isVirusFree(node))) {
                    //this component contains both a virus and a regular node
                    return false;
                }
                checked[node] = true;
                vector<int> neighbors = g.getNeighbors(node);
                //we will check all of the node's neighbors, that had not been checked yet
                int size = neighbors.size();
                for(int j=0; j<size; j++)
                    if(!checked[neighbors[j]])
                        bfsQ.push(neighbors[j]);
            }
        }
    }
    return true; //we checked all components and they all met the termination conditions
}

void Session::output() {
    nlohmann::json j;
    j["graph:"] = g.getEdges();
    int size = g.size();
    for(int i=0;i<size; i++){
        if(g.isInfected(i))
            j["list of infected nodes:"]+=i;
    }
    ofstream i("./output.json");
    i<<j;
}
