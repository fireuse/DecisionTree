cmake_minimum_required(VERSION 3.24)
project(DecisionTree)

set(CMAKE_CXX_STANDARD 17)
find_package(Catch2 3 REQUIRED)

add_executable(Tests tests/test_dataset.cpp  tests/test_node.cpp src/Dataset.cpp src/TreeNode.cpp)
target_link_libraries(Tests Catch2::Catch2WithMain)
add_executable(DecisionTree main.cpp src/DecisionTree.cpp include/DecisionTree.h src/TreeNode.cpp include/TreeNode.h src/Dataset.cpp include/Dataset.h tests/test_dataset.cpp tests/DataGenerator.h)
