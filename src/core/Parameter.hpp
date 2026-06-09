#pragma once
#include <string>

/** UI widget type used when editing a simulation parameter. */
enum class ParameterType
{
    Integer,
    Double
};

/** Runtime-adjustable simulation parameter with bounds and display metadata. */
struct Parameter{
    std::string name;       /**< Identifier passed to Simulation::setParameter(). */
    double value;           /**< Current value (integer parameters stored as double). */
    double minValue;        /**< Lower bound for UI sliders/spin boxes. */
    double maxValue;        /**< Upper bound for UI sliders/spin boxes. */
    ParameterType type;     /**< Controls integer vs floating-point editing in the UI. */
};