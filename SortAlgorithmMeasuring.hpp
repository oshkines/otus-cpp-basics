#include <vector>
#pragma once

    enum TypeSort{
        EnumSortAlgorithmMeasuring,
        EnumQuickSortAlgorithmMeasuring
    };

std::vector<double>  SortAlgorithmMeasuring(std::vector<int> &vec, int iteration_count, enum TypeSort ts);
//enum class TypeSort;
