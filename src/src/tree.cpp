#include "../include/tree.h"

int treeNode::cnt = 0;
/*
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stdio.h>

using namespace std;

typedef union nodeValue
{
    int intValue;
    double dblValue;
    char charValue;
    string strValue;
} val;

class treeNode
{
public:
    int idx;
    //val value;
    string name;
    vector<treeNode*> child;
    
    static int cnt;

    treeNode() {}
    treeNode(string name): idx((this->cnt)++), name(name) {}
    ~treeNode() {}

    void addChild(treeNode* node) {
        child.push_back(node);
    }

    void prtIdx() {
      cout << this->idx;
    }

    void prtNode() {
      cout<<"Name: "<<this->name<< ", index: " << this->idx <<", child:[";
      for(auto i = 0; i < child.size(); i++) {
          if(i == 0) cout << child[i]->idx;
          else cout << ";" << child[i]->idx;
      }
      cout << "] ";
    }
};

class parseTree
{
public:
    treeNode* root;

    parseTree() {}
    parseTree(treeNode* root):root(root) {}
    ~parseTree() {}
    
    void prtTree() {
        queue<treeNode*> nodeQueue;
        treeNode *node = root;
        root->prtNode();
        cout << endl;
        nodeQueue.push(root);
        while(!nodeQueue.empty()) {
            node = nodeQueue.front();
            nodeQueue.pop();
            for(auto i = 0; i < node->child.size(); i++) {
              node->child[i]->prtNode();
              cout << "[Parent Idx: ";
              node->prtIdx();
              cout << "]" << endl;
              nodeQueue.push(node->child[i]);
            }
        }
    }
};
*/

/*
int main(void)
{
    int idx = 0;
    treeNode n1 = treeNode(idx++, "doc");
    treeNode n2 = treeNode(idx++, "stmt");
    treeNode n3 = treeNode(idx++, "func");
    treeNode n4 = treeNode(idx++, "block");

    n1.addChild(&n2);
    n1.addChild(&n3);
    n2.addChild(&n4);
    
    parseTree T = parseTree(&n1);
    T.prtTree();

    return 0;
}
*/