#ifndef DECISIONTREE_DECISIONTREE_H
#define DECISIONTREE_DECISIONTREE_H


#include "TreeNode.h"

class DecisionTree {
    TreeNode *parent;
    int maxDepth;
    float purity;
public:
    explicit DecisionTree(int maxDepth, float purity);

    void initialize(Dataset &data);
    std::vector<int> predict(const std::vector<std::vector<double>> &data);
    int getDepth();
};


#endif //DECISIONTREE_DECISIONTREE_H
