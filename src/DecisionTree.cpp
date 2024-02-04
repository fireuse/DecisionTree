#include "../include/DecisionTree.h"

int DecisionTree::getDepth() {
    return parent->getDepth();
}

int DecisionTree::predict(std::vector<double> &data) {
    return parent->predict(data);
}

void DecisionTree::initialize(Dataset &data) {
    parent->initialize(data, maxDepth);
}

DecisionTree::DecisionTree(int maxDepth) : maxDepth(maxDepth) {
    parent = new TreeNode(0);
}
