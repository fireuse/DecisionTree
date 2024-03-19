#ifndef DECISIONTREE_DATAGENERATOR_H
#define DECISIONTREE_DATAGENERATOR_H

#include <functional>
#include <random>
#include "../include/Dataset.h"

class VectorGenerator {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_real_distribution<double> dist;

public:
    explicit VectorGenerator(std::tuple<double, double> range) : dist(std::get<0>(range), std::get<1>(range)) {};

    std::vector<double> generate(int n) {
        std::vector<double> out(n);

        std::generate(out.begin(), out.end(), [this]() {
            return dist(mersenne_engine);
        });
        return out;
    }
};

class DataGenerator {
    std::function<bool(std::vector<double>)> classifier;
    VectorGenerator generator;
    int features;
    int len;
public:
    DataGenerator(int n_features, int len, std::function<bool(std::vector<double>)> label,
                  std::tuple<double, double> range) : generator(range) {
        classifier = label;
        this->features = n_features;
        this->len = len;
    }

    Dataset generate(std::function<double(std::unordered_map<int, float>)> gain) {
        std::vector<std::vector<double>> data;
        std::vector<int> labels;
        for (int i = 0; i <= len; ++i) {
            data.push_back(generator.generate(features));
        }
        for (const auto &i: data) {
            labels.push_back(classifier(i));
        }
        return {labels, data, gain};
    }
};

#endif //DECISIONTREE_DATAGENERATOR_H
