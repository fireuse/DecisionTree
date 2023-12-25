#ifndef DECISIONTREE_TREENODE_H
#define DECISIONTREE_TREENODE_H


#include <vector>
#include <tuple>
#include "Dataset.h"


struct Split {
    int axis;
    double compare;
    double goodness;
    std::tuple<Dataset, Dataset> dataset;

    Split(int axis, double compare, double goodness, std::tuple<Dataset, Dataset> dataset);
};

class Node {
public:
    virtual void
    initialize(Dataset dataset, int maxDepth) = 0;

    virtual int predict(std::vector<double> &data) = 0;

    virtual ~Node() = default;
};

class TreeNode : Node {
    Node *left;
    Node *right;
    int axis;
    double compare;
    int depth;

    static Split
    createSplit(Dataset &data, double value, int ax);

    static bool checkPurity(Dataset &data);

    static Node *createNode(Dataset &data, int currentDepth);

public:
    explicit TreeNode(int depth);

    void initialize(Dataset dataset, int maxDepth) override;

    int predict(std::vector<double> &data) override;

    ~TreeNode() override;
};

class LeafNode : public Node {
    int returnType;
public:
    void initialize(Dataset dataset, int maxDepth) override;

    int predict(std::vector<double> &data) override;

    ~LeafNode() override = default;
};

#endif //DECISIONTREE_TREENODE_H
