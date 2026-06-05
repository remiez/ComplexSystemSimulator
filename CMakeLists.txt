cmake_minimum_required(VERSION 3.20)

project(ComplexSystemSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Charts)
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

add_executable(ComplexSystemSimulator
    src/main.cpp

    src/gui/MainWindow.cpp
    src/gui/MainWindow.hpp
    src/gui/ParameterPanel.cpp
    src/gui/ParameterPanel.hpp
    src/gui/PlotWidget.cpp
    src/gui/PlotWidget.hpp

    src/rendering/SFMLWidget.cpp
    src/rendering/SFMLWidget.hpp

    src/simulations/IsingSimulation.cpp
    src/simulations/IsingSimulation.hpp
    src/simulations/PercolationSimulation.cpp
    src/simulations/PercolationSimulation.hpp
    src/simulations/QuantSimulation.cpp
    src/simulations/QuantSimulation.hpp
    src/simulations/KuramotoSimulation.cpp
    src/simulations/KuramotoSimulation.hpp

    src/core/SimulationManager.cpp
    src/core/SimulationManager.hpp
    src/core/SimulationManager.cpp
    src/core/Simulation.hpp
)

target_link_libraries(ComplexSystemSimulator
    PRIVATE
    Qt5::Widgets
    Qt5::Charts

    sfml-graphics
    sfml-window
    sfml-system
)