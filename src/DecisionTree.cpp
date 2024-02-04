#include "../include/DecisionTree.h"

int DecisionTree::getDepth() {
    return parent->getDepth();
}

void DecisionTree::initialize(Dataset &data) {
    parent->initialize(data, maxDepth);
}

DecisionTree::DecisionTree(int maxDepth) : maxDepth(maxDepth) {
    parent = new TreeNode(0);
}

std::vector<int> DecisionTree::predict(const std::vector<std::vector<double>> &data) {
    std::vector<int> out;
    for (const std::vector<double> &i: data) {
        out.push_back(parent->predict(i));
    }
    return out;
}

