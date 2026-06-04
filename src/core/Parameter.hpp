#pragma once
#include <string>

enum class ParameterType
{
    Integer,
    Double
};

struct Parameter{
    std::string name;
    double value;
    double minValue;
    double maxValue;
    ParameterType type;
};