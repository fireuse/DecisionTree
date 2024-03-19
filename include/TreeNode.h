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
    initialize(Dataset dataset, int maxDepth, float targetPurity) = 0;

    virtual int predict(const std::vector<double> &data) = 0;

    virtual int getDepth() = 0;

    virtual ~Node() = default;
};

class TreeNode : public Node {
    Node *left;
    Node *right;
    int axis;
    double compare;
    int depth;
    float targetPurity;

    static Split
    createSplit(Dataset &data, double value, int ax);

    bool checkPurity(Dataset &data) const;

    Node *createNode(Dataset &data, int currentDepth);

public:
    explicit TreeNode(int depth);

    void initialize(Dataset dataset, int maxDepth, float targetPurity) override;

    int predict(const std::vector<double> &data) override;

    int getDepth() override;

public:
    ~TreeNode() override;
};

class LeafNode : public Node {
    int returnType = 0;
    int depth;
public:
    explicit LeafNode(int depth);
    void initialize(Dataset dataset, int maxDepth, float targetPurity) override;

    int predict(const std::vector<double> &data) override;

    int getDepth() override;

    ~LeafNode() override = default;
};

#endif //DECISIONTREE_TREENODE_H
