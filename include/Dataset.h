#ifndef DECISIONTREE_DATASET_H
#define DECISIONTREE_DATASET_H

#include <tuple>
#include <vector>
#include <unordered_map>

class Dataset {
    std::vector<int> labels;
    std::vector<std::vector<double>> data;
public:
    Dataset(const std::vector<int> &labels, const std::vector<std::vector<double>> &data);

    std::tuple<Dataset, Dataset> split(int axis, double value);

    const std::vector<std::vector<double>> &getData();

    const std::vector<int> &getLabels();

    std::unordered_map<int, float> frequencies();
};


#endif //DECISIONTREE_DATASET_H
