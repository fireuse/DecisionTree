#include <catch2/catch_all.hpp>
#include "../include/TreeNode.h"

Dataset makeData(int len, double percent) {
    std::vector<std::vector<double>> data;
    std::vector<int> labels;

    for (int i = 0; i < len * percent; ++i) {
        data.push_back({0});
        labels.push_back(1);
    }
    for (int i = 0; i < len * (1 - percent); ++i) {
        data.push_back({1});
        labels.push_back(0);
    }

    return {labels, data, gain::entropyGain};
}

TEST_CASE("LEAF_NODE") {
    double percent = GENERATE(0, 0.2, 0.4);
    int len = 10000;

    LeafNode l(0);
    l.initialize(makeData(len, percent), 100, 0.9);

    std::vector<double> trueValue{1};
    REQUIRE(l.predict(trueValue) == 0);

    std::vector<double> falseValue{0};
    REQUIRE(l.predict(falseValue) == 0);

    REQUIRE(l.getDepth() == 0);
}

TEST_CASE("TREE_NODE") {
    Dataset d = makeData(1000, GENERATE(0.2, 0.5, 0.7));
    TreeNode t(0);
    t.initialize(d, 10, 0.9);

    std::vector<double> trueValue{1};
    REQUIRE(t.predict(trueValue) == 0);

    std::vector<double> falseValue{0};
    REQUIRE(t.predict(falseValue) == 1);
}