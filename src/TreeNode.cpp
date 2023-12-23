#include "../include/TreeNode.h"
#include <algorithm>
#include <utility>
#include <tuple>

int TreeNode::predict(std::vector<double> &data) {

    return 0;
}

void TreeNode::initialize(Dataset dataset, int maxDepth) {
    std::vector<Split> splits;
    auto &data = dataset.getData();
    auto size = data.size();
    for (int i = 0; i <= size; ++i) {
        for (int j = 0; j <= data[0].size(); ++j) {
            splits.push_back(TreeNode::createSplit(dataset, data[i][j], j));
        }
    }
    Split best = *std::min_element(splits.begin(), splits.end(),
                                   [](const Split &x, const Split &y) { return x.goodness < y.goodness; });
    compare = best.compare;
    axis = best.axis;
    auto [dataLeft, dataRight] = best.dataset;
    if (depth < maxDepth) {
        left = new TreeNode(depth + 1);
        right = new TreeNode(depth + 1);
    } else {
        left = new LeafNode();
        right = new LeafNode();
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
    return {0, 0, 0, {dataset, dataset}};
}

void LeafNode::initialize(Dataset dataset, int maxDepth) {

}

int LeafNode::predict(std::vector<double> &data) {
    return returnType;
}


Split::Split(int axis, double compare, double goodness, std::tuple<Dataset, Dataset> dataset) : axis(axis),
                                                                                                compare(compare),
                                                                                                goodness(goodness),
                                                                                                dataset(std::move(
                                                                                                        dataset)) {}
