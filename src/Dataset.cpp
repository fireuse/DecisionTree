#include <cmath>
#include <utility>
#include "../include/Dataset.h"

Dataset::Dataset(const std::vector<int> &labels, const std::vector<std::vector<double>> &data,
                 std::function<double(std::unordered_map<int, float>)> gain) : labels(labels),
                                                                               data(data), gain(gain) {}

std::tuple<Dataset, Dataset> Dataset::split(int axis, double value) {
    std::vector<std::vector<double>> dataLeft;
    std::vector<std::vector<double>> dataRight;
    std::vector<int> labelsLeft;
    std::vector<int> labelsRight;
    unsigned long size = data.size();
    for (int i = 0; i < size; ++i) {
        if (data[i][axis] < value) {
            dataRight.push_back(data[i]);
            labelsRight.push_back(labels[i]);
        } else {
            dataLeft.push_back(data[i]);
            labelsLeft.push_back(labels[i]);
        }
    }
    return {Dataset(labelsLeft, dataLeft, gain), Dataset(labelsRight, dataRight, gain)};
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

double Dataset::gainFunction() {
    return gain(frequencies());
}

int Dataset::countCorrect(std::vector<int> &test) {
    int correctCount = 0;
    for (int i = 0; i <= labels.size(); ++i) {
        correctCount += test[i] == labels[i];
    }
    return correctCount;
}

std::tuple<Dataset, Dataset> Dataset::divide(float percent) {
    int split = labels.size() * percent;
    auto splittingPointLabels = labels.begin() + split;
    auto splittingPointData = data.begin() + split;
    std::vector<int> labelsLeft(labels.begin(), splittingPointLabels);
    std::vector<int> labelsRight(splittingPointLabels, labels.end());
    std::vector<std::vector<double>> dataLeft(data.begin(), splittingPointData);
    std::vector<std::vector<double>> dataRight(splittingPointData, data.end());
    return {Dataset(labelsLeft, dataLeft, gain), Dataset(labelsRight, dataRight, gain)};
}

double gain::giniCriterion(const std::unordered_map<int, float> &data) {
    double gini = 0;
    for(auto [label, frequency] : data){
        gini += frequency * frequency;
    }
    return 1 - gini;
}

double gain::entropyGain(const std::unordered_map<int, float>& data) {
    double entropy = 0;
    for(auto [label, frequency] : data){
        entropy += frequency*log2(frequency);
    }
    return -entropy;
}
