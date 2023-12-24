#include <catch2/catch_all.hpp>
#include <iostream>
#include "DataGenerator.h"

TEST_CASE("Dataset_Test") {
    std::vector<std::vector<double>> data;
    std::vector<int> labels;
    for (int i = 0; i < 100; ++i) {
        data.push_back({1});
        labels.push_back(1);
    }
    for (int i = 0; i < 100; ++i) {
        data.push_back({0});
        labels.push_back(0);
    }
    //frequency test
    Dataset d(labels, data);
    REQUIRE(d.getData().size() == 200);
    auto frequencies = d.frequencies();
    REQUIRE(frequencies.at(0) == 0.5);
    REQUIRE(frequencies.at(1) == 0.5);

    //split test
    auto [l, r] = d.split(0, 0.5);
    REQUIRE(r.getData().size() == 100);
    REQUIRE(l.getData().size() == 100);
    REQUIRE(r.frequencies().at(1) == 1);
    REQUIRE(l.frequencies().at(0) == 1);

    REQUIRE(d.entropy() == 1);
}