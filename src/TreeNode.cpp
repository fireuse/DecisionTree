#include "../include/TreeNode.h"
#include <algorithm>
#include <utility>
#include <tuple>

int TreeNode::predict(const std::vector<double> &data) {
    if (data[axis] < compare) {
        return right->predict(data);
    }
    return left->predict(data);
}

void TreeNode::initialize(Dataset dataset, int maxDepth, float purity, bool reinit) {
    if (reinit) {
        auto [l, r] = dataset.split(axis, compare);
        left->initialize(l, maxDepth, purity, reinit);
        right->initialize(r, maxDepth, purity, reinit);
        return;
    }
    targetPurity = purity;
    std::vector<Split> splits;
    auto &data = dataset.getData();
    auto size = data.size();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < data[0].size(); ++j) {
            splits.push_back(TreeNode::createSplit(dataset, data[i][j], j));
        }
    }
    Split best = *std::max_element(splits.begin(), splits.end(),
                                   [](const Split &x, const Split &y) { return x.goodness < y.goodness; });
    compare = best.compare;
    axis = best.axis;
    auto [dataLeft, dataRight] = best.dataset;
    if (depth < maxDepth) {
        left = createNode(dataLeft, depth);
        right = createNode(dataRight, depth);
    } else {
        left = new LeafNode(depth);
        right = new LeafNode(depth);
    }
    left->initialize(dataLeft, maxDepth, purity, reinit);
    right->initialize(dataRight, maxDepth, purity, reinit);
}

TreeNode::TreeNode(int depth) : depth(depth) {}

TreeNode::~TreeNode() {
    delete left;
    delete right;
}

Split TreeNode::createSplit(Dataset &dataset, double value, int ax) {
    auto [dataLeft, dataRight] = dataset.split(ax, value);
    unsigned long parentSize = dataset.getData().size();
    double goodness = dataset.gainFunction() - dataLeft.getData().size() / parentSize * dataLeft.gainFunction() -
                      dataRight.getData().size() / parentSize * dataRight.gainFunction();
    return {ax, value, goodness, {dataLeft, dataRight}};
}

bool TreeNode::checkPurity(Dataset &data) const {
    for (auto [label, freq]: data.frequencies()) {
        if (freq > targetPurity) {
            return true;
        }
    }
    return false;
}

Node *TreeNode::createNode(Dataset &data, int currentDepth) {
    if (checkPurity(data)) {
        return new LeafNode(currentDepth);
    } else {
        return new TreeNode(currentDepth + 1);
    }
}

int TreeNode::getDepth() {
    return std::max(left->getDepth(), right->getDepth());
}

TreeNode::TreeNode(TreeNode *tr) {
    axis = tr->axis;
    depth = tr->depth;
    compare = tr->compare;
    targetPurity = tr->targetPurity;
    if (dynamic_cast<LeafNode *>(tr->left)) {
        left = new LeafNode(*(LeafNode *) tr->left);
    } else {
        left = new TreeNode((TreeNode *) tr->left);
    }
    if (dynamic_cast<LeafNode *>(tr->right)) {
        right = new LeafNode(*(LeafNode *) tr->right);
    } else {
        right = new TreeNode((TreeNode *) tr->right);
    }
}

std::vector<TreeNode *> TreeNode::getNodesAt(int n) {
    std::vector<TreeNode *> results;
    if (!dynamic_cast<LeafNode *>(left)) {
        if (n == 0) {
            auto lr = ((TreeNode *) left)->getNodesAt(n - 1);
            results.insert(results.end(), lr.begin(), lr.end());
        } else {
            results.push_back((TreeNode *) left);
        }
    }
    if (!dynamic_cast<LeafNode *>(right)) {
        if (n == 0) {
            auto rr = ((TreeNode *) right)->getNodesAt(n - 1);
            results.insert(results.end(), rr.begin(), rr.end());
        } else {
            results.push_back((TreeNode *) right);
        }
    }
    return results;
}


void LeafNode::initialize(Dataset dataset, int maxDepth, float targetPurity, bool reinit) {
    if (dataset.getData().empty()) {
        returnType = -1;
        return;
    }
    auto frequencies = dataset.frequencies();
    returnType = std::get<0>(
            *std::max_element(frequencies.begin(), frequencies.end(), [](const auto x1, const auto x2) {
                return std::get<1>(x1) < std::get<1>(x2);
            }));
}

int LeafNode::predict(const std::vector<double> &data) {
    return returnType;
}

int LeafNode::getDepth() {
    return depth;
}

LeafNode::LeafNode(int depth) : depth(depth) {}


Split::Split(int axis, double compare, double goodness, std::tuple<Dataset, Dataset> dataset) : axis(axis),
                                                                                                compare(compare),
                                                                                                goodness(goodness),
                                                                                                dataset(std::move(
                                                                                                        dataset)) {}
