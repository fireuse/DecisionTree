#include "../include/TreeNode.h"
#include <algorithm>
#include <utility>
#include <tuple>

int TreeNode::predict(const std::vector<double> &data) {
    if (data[axis] < compare){
        return right->predict(data);
    }
    return left->predict(data);
}

void TreeNode::initialize(Dataset dataset, int maxDepth) {
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
    left->initialize(dataLeft, maxDepth);
    right->initialize(dataRight, maxDepth);
}

TreeNode::TreeNode(int depth) : depth(depth) {}

TreeNode::~TreeNode() {
    delete left;
    delete right;
}

Split TreeNode::createSplit(Dataset &dataset, double value, int ax) {
    auto [dataLeft, dataRight] = dataset.split(ax, value);
    unsigned long parentSize = dataset.getData().size();
    double goodness = dataset.entropy() - dataLeft.getData().size() / parentSize * dataLeft.entropy() -
                      dataRight.getData().size() / parentSize * dataRight.entropy();
    return {ax, value, goodness, {dataLeft, dataRight}};
}

bool TreeNode::checkPurity(Dataset &data) {
    for (auto [label, freq]: data.frequencies()) {
        if (freq > 0.99) {
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

void LeafNode::initialize(Dataset dataset, int maxDepth) {
    if (dataset.getData().empty()){
        returnType = -1;
        return;
    }
    auto frequencies = dataset.frequencies();
    returnType = std::get<0>(*std::max_element(frequencies.begin(),frequencies.end(), [](const auto x1, const auto x2){
        return std::get<1>(x1) < std::get<1>(x2);
    }));
}

int LeafNode::predict(const std::vector<double> &data) {
    return returnType;
}

int LeafNode::getDepth() {
    return depth;
}

LeafNode::LeafNode(int depth) : depth(depth){}


Split::Split(int axis, double compare, double goodness, std::tuple<Dataset, Dataset> dataset) : axis(axis),
                                                                                                compare(compare),
                                                                                                goodness(goodness),
                                                                                                dataset(std::move(
                                                                                                        dataset)) {}
