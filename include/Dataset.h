#ifndef DECISIONTREE_DATASET_H
#define DECISIONTREE_DATASET_H

#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>

class Dataset {
    std::vector<int> labels;
    std::vector<std::vector<double>> data;
    std::function<double(std::unordered_map<int, float>)> gain;
public:
    Dataset(const std::vector<int> &labels, const std::vector<std::vector<double>> &data,
            std::function<double(std::unordered_map<int, float>)> gain);

    std::tuple<Dataset, Dataset> split(int axis, double value);

    const std::vector<std::vector<double>> &getData();

    const std::vector<int> &getLabels();

    std::unordered_map<int, float> frequencies();

    double gainFunction();

    int countCorrect(std::vector<int>& labels);

    std::tuple<Dataset, Dataset> divide(float percent);
};

namespace gain {
    double entropyGain(const std::unordered_map<int, float>& data);

    double giniCriterion(const std::unordered_map<int, float> &data);
}

#endif //DECISIONTREE_DATASET_H
