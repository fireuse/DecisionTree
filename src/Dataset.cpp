#include <cmath>
#include "../include/Dataset.h"

Dataset::Dataset(const std::vector<int> &labels, const std::vector<std::vector<double>> &data) : labels(labels),
                                                                                                 data(data) {}

std::tuple<Dataset, Dataset> Dataset::split(int axis, double value) {
    std::vector<std::vector<double>> dataLeft;
    std::vector<std::vector<double>> dataRight;
    std::vector<int> labelsLeft;
    std::vector<int> labelsRight;
    unsigned long size = data.size();
    for (int i = 0; i < size; ++i) {
        if (data[i][axis] > value) {
            dataRight.push_back(data[i]);
            labelsRight.push_back(labels[i]);
        } else {
            dataLeft.push_back(data[i]);
            labelsLeft.push_back(labels[i]);
        }
    }
    return {Dataset(labelsLeft, dataLeft), Dataset(labelsRight, dataRight)};
}

const std::vector<std::vector<double>> &Dataset::getData() {
    return data;
}

const std::vector<int> &Dataset::getLabels() {
    return labels;
}


std::unordered_map<int, float> Dataset::frequencies() {
    std::unordered_map<int, int> counts;
    for (int i: labels) {
        ++counts[i];
    }
    std::unordered_map<int, float> out;
    unsigned long size = labels.size();
    for (auto &[i, count]: counts) {
        out[i] = (float) count / size;
    }
    return out;
}

double Dataset::entropy() {
    double entropy = 0;
    for(auto [label, frequency] : frequencies()){
        entropy += frequency*log2(frequency);
    }
    return -entropy;
}
