#include "../include/Tree.h"
#include <iostream>
#include "queue"
#include "../include/Session.h"
using namespace std;

Tree::Tree(int rootLabel):node(rootLabel), children() {};

Tree::Tree(const Tree& other):node(other.node), children(){// copy const
    int size = other.children.size();
    for(int i=0; i < size; i++)
        children.push_back(other.children[i]->clone());
};

Tree::Tree(Tree &&other):node(other.node), children(){//move cnst
    int size= other.children.size();
    for(int i=0;i < size;i++){
        children.push_back(other.children[i]);
    }
    other.children.clear();
}

Tree& Tree::operator=(const Tree& t2){//assignment operator
    if(this == &t2)
        return *this;
    clear();
    int size = t2.children.size();
    for(int i=0; i < size; i++)
        children.push_back(t2.children[i]->clone());
    node = t2.node;
    return *this;
};

Tree& Tree::operator=(Tree &&other) {//move operator
    if(this!= &other){
        clear();
        int size = other.children.size();
        for(int i=0;i<size;i++){
            children.push_back(other.children[i]);
        }
        node=other.node;
        other.children.clear();
    }
    return *this;
}

Tree::~Tree() {clear();}

void Tree::clear(){
    //delete all children
    int size = children.size();
    for(int i=0; i<size; i++)
        delete children[i];
};

int Tree::getNode() const {return node;}

vector<Tree*> Tree::getChildren() {return children;};

void Tree::addChild(const Tree &child) {
    children.push_back(child.clone());
};

void Tree::addChild(Tree *child) {
    children.push_back(child);
}

Tree* Tree::createTree(const Session &session, int rootLabel) {
    TreeType type = session.getTreeType();// check type
    Tree* ans;
    switch(type){
        case(Cycle):
            ans = new CycleTree(rootLabel , session.getCycle());
            break;
        case(MaxRank):
            ans = new MaxRankTree(rootLabel);
            break;
        case(Root):
            ans = new RootTree(rootLabel);
            break;
    }
    return ans;// return the tree whit the specific type
}

CycleTree::CycleTree(int rootLabel, int currCycle):Tree(rootLabel),currCycle(currCycle) {};

int CycleTree::traceTree() {
    Tree* curr = this;
    while ((currCycle > 0) & (curr->getChildren().size()!=0)) {
        curr = curr->getChildren()[0]; //move to the smallest child
        currCycle--;
    }
    return curr->getNode();
}

MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){}

int MaxRankTree::MaxDeg(Tree* tree) {
    int curr = tree->getChildren().size();
    vector<Tree*> treeChildren = tree->getChildren();
    int size = treeChildren.size();
    for(int i=0;i<size;i++){
        // check in recursion which node have the biggest amount of children
        curr = std::max(curr, MaxDeg(treeChildren[i]));
    }
    return curr;


}
int MaxRankTree::traceTree() {
    int max = MaxDeg(this);
    queue<Tree*> q;
    q.push(this);
    while(!q.empty()){
        //search for the first(leftmost, least deep) legal node whit max degree size
        Tree* curr = q.front();
        q.pop();
	int size = curr->getChildren().size();
        if(size == max){
            return curr->getNode();
        }
        else{
            int size = curr->getChildren().size();
            for(int i=0; i< size; i++){
                q.push(curr->getChildren()[i]);
                // didnt find the node push the next children
            }
        }
    }
    return 0; // never reach this line
}

RootTree::RootTree(int rootLabel): Tree(rootLabel){};

int RootTree::traceTree() {
    return this->getNode();
};

Tree* MaxRankTree::clone() const {return new MaxRankTree(*this);};

Tree* CycleTree::clone() const {return new CycleTree(*this);};

Tree* RootTree::clone() const {return new RootTree(*this);};

CycleTree::CycleTree(const CycleTree &t2):Tree(t2),currCycle(t2.currCycle) {};

MaxRankTree::MaxRankTree(const MaxRankTree &t2):Tree(t2) {};

RootTree::RootTree(const RootTree &t2):Tree(t2) {};

CycleTree::~CycleTree() {};

MaxRankTree::~MaxRankTree() {};

RootTree::~RootTree() {};

Tree& CycleTree::operator = (CycleTree& other){
    Tree::operator=(other);
    if(this!= &other){
        currCycle = other.currCycle;
    }
    return *this;
}

Tree& RootTree::operator=(RootTree &other){
    Tree::operator=(other);
    return *this;
}

Tree& MaxRankTree::operator=(MaxRankTree &other){
    Tree::operator=(other);
    return *this;
}


Tree& CycleTree::operator=(CycleTree &&other) {
    Tree::operator=(std::move(other));
    if(this!= &other){
        currCycle = other.currCycle;
    }
    return *this;
}

Tree& RootTree::operator=(RootTree &&other){
    Tree::operator=(std::move(other));
    return *this;
}

Tree& MaxRankTree::operator=(MaxRankTree &&other){
    Tree::operator=(std::move(other));
    return *this;
}