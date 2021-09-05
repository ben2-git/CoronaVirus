#ifndef UNTITLED_TREE_H
#define UNTITLED_TREE_H
#include <vector>
using namespace std;

class Session;

class Tree{

public:
    Tree(int rootLabel);
    Tree(const Tree& other);
    Tree& operator = (const Tree& t2);
    virtual ~Tree();
    Tree(Tree&& other);
    Tree& operator = (Tree&& other);
    int getNode() const;
    vector<Tree*> getChildren();
    void addChild(const Tree& child);
    void addChild(Tree* child);
    void clear();
    virtual Tree* clone() const = 0 ;
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;

private:
    int node;
    std::vector<Tree*> children;

};

class CycleTree: public Tree{

public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    CycleTree(const CycleTree& t2);
    Tree& operator = (CycleTree& other);
    Tree& operator = (CycleTree&& other);
    virtual Tree* clone() const;
    virtual ~CycleTree();

private:
    int currCycle;

};

class MaxRankTree: public Tree{

public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    int MaxDeg(Tree*);
    MaxRankTree(const MaxRankTree& t2);
    Tree& operator = (MaxRankTree& other);
    Tree& operator = (MaxRankTree&& other);
    virtual Tree* clone() const;
    virtual ~MaxRankTree();
};

class RootTree: public Tree{

public:
    RootTree(int rootLabel);
    virtual int traceTree();
    RootTree(const RootTree& t2);
    Tree& operator = (RootTree& other);
    Tree& operator = (RootTree&& other);
    virtual Tree* clone() const;
    virtual ~RootTree();
};
#endif //UNTITLED_TREE_H
