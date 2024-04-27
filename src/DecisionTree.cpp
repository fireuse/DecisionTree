#include <iostream>
#include "../include/DecisionTree.h"

int DecisionTree::getDepth() {
    return parent->getDepth();
}

void DecisionTree::initialize(Dataset &data) {
    auto [training, pruning] = data.divide(0.8);
    parent->initialize(training, maxDepth, purity, false);
    std::cout << "Before Prune Depth: " << getDepth() << " Leafs: " << countLeaves(parent) << std::endl;
    prune(pruning, training);
    std::cout << "After Prune Depth: " << getDepth() << " Leafs: " << countLeaves(parent) << std::endl;
}

DecisionTree::DecisionTree(int maxDepth, float purity) : maxDepth(maxDepth), purity(purity) {
    parent = new TreeNode(0);
}

std::vector<int> DecisionTree::predict(const std::vector<std::vector<double>> &data) {
    std::vector<int> out;
    for (const std::vector<double> &i: data) {
        out.push_back(parent->predict(i));
    }
    return out;
}

int DecisionTree::countLeaves(Node *node) {
    if (dynamic_cast<LeafNode *>(node)) {
        return 1;
    }
    return countLeaves(((TreeNode *) node)->left) + countLeaves(((TreeNode *) node)->right);
}

int DecisionTree::leafCount() {
    return countLeaves(parent);
}

void DecisionTree::prune(Dataset &pruning, Dataset &training) {
    std::vector<DecisionTree *> pruned;
    int currentDepth = getDepth() - 1;
    if(currentDepth <= 0) return;
    for (int i = 0; i < currentDepth; ++i) {
        std::vector<TreeNode *> nodes = parent->getNodesAt(i);
        for (TreeNode *n: nodes) {
            auto *prevNode = (TreeNode *) n->right;
            auto *l = new LeafNode(prevNode->depth);
            n->right = l;
            pruned.push_back(new DecisionTree(maxDepth, purity, new TreeNode(parent)));
            n->right = prevNode;
            prevNode = (TreeNode *) n->left;
            n->left = l;
            pruned.push_back(new DecisionTree(maxDepth, purity, new TreeNode(parent)));
            n->left = prevNode;
            delete l;
        }
    }
    int leafs = countLeaves(parent);
    double acc = evalAccuracy(pruning);
    double error = 1;
    auto best = pruned.begin();
    for (auto i = pruned.begin(); i != pruned.end(); ++i) {
        double treeAcc = (*i)->evalAccuracy(pruning);
        int treeLeafs = (*i)->countLeaves((*i)->parent);
        double treeErr = (treeAcc - acc)/(leafs-treeLeafs);
        if(treeErr < error){
            error = treeErr;
            best = i;
        }
    }
    delete parent;
    parent = new TreeNode((*best)->parent);
    parent->initialize(training, maxDepth, purity, true);
    for (DecisionTree *i: pruned) {
        delete i;
    }
}

DecisionTree::~DecisionTree() {
    delete parent;
}

DecisionTree::DecisionTree(int maxDepth, float purity, TreeNode *oldParent) : maxDepth(maxDepth), purity(purity),
                                                                              parent(oldParent) {}

double DecisionTree::evalAccuracy(Dataset &data) {
    std::vector<int> labels = predict(data.getData());
    return (float) data.countCorrect(labels)/ (float) labels.size();
}

