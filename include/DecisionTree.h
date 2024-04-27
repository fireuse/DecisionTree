#ifndef DECISIONTREE_DECISIONTREE_H
#define DECISIONTREE_DECISIONTREE_H


#include "TreeNode.h"

class DecisionTree {
    friend TreeNode;
    TreeNode *parent;
    int maxDepth;
    float purity;
    int countLeaves(Node* node);
    void prune(Dataset& pruning, Dataset& training);
    DecisionTree(int maxDepth, float purity, TreeNode* oldParent);
public:
    explicit DecisionTree(int maxDepth, float purity);
    void initialize(Dataset &data);
    double evalAccuracy(Dataset& data);
    std::vector<int> predict(const std::vector<std::vector<double>> &data);
    int getDepth();
    int leafCount();
    ~DecisionTree();
};


#endif //DECISIONTREE_DECISIONTREE_H
