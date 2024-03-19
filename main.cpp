#include <iostream>
#include "include/DecisionTree.h"
#include "tests/DataGenerator.h"

bool inDistance(const std::vector<double> &x, double distance) {
    double radius = distance * distance;
    double sum = 0;
    for (double i: x) {
        sum += i * i;
    }
    return sum <= radius;
}

void runCheck(int features, double min, double max, double distance,
              std::function<double(std::unordered_map<int, float>)> gain) {
    std::cout << "Run for " << features << std::endl;
    DecisionTree tree(100, 0.9);
    DataGenerator gen(3, 1000, [distance](const std::vector<double> &x) { return inDistance(x, distance); },
                      {min, max});
    Dataset d = gen.generate(gain);
    tree.initialize(d);
    std::cout << "Tree Depth " << tree.getDepth() << std::endl;
    Dataset test = gen.generate(gain);
    std::vector<int> labels = tree.predict(test.getData());
    int correctCount = 0;
    for (int i = 0; i <= labels.size(); ++i) {
        correctCount += labels[i] == test.getLabels()[i];
    }
    std::cout << "Accuracy " << (float) correctCount / (float) labels.size() << std::endl;
}

int main() {
    runCheck(1, -1, 1, 0.5, gain::giniCriterion);
    runCheck(2, -1, 1, 1, gain::giniCriterion);
    runCheck(3, -1, 1, 1, gain::entropyGain);
}
