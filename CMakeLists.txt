cmake_minimum_required(VERSION 3.24)
project(DecisionTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(DecisionTree main.cpp src/DecisionTree.cpp include/DecisionTree.h src/TreeNode.cpp include/TreeNode.h src/Dataset.cpp include/Dataset.h)
