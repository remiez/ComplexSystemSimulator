# Complex Systems and Quant Monte Carlo Simulator

A C++20 desktop application for simulating complex systems and quantitative finance models with real-time visualization.

The project combines statistical physics simulations with quantitative Monte Carlo methods in a single modular framework built with Qt and SFML.

---

## Features

### Ising Model

* 2D Ising model
* Metropolis Monte Carlo algorithm
* Adjustable temperature
* Adjustable external magnetic field
* Real-time visualization of spin configurations
* Energy and magnetization statistics

### Percolation

* Site percolation on a square lattice
* Adjustable occupation probability
* Cluster identification
* Percolation detection
* Real-time visualization

### Kuramoto Model

* Coupled phase oscillators
* Adjustable coupling strength
* Adjustable natural frequency spread
* Synchronization order parameter
* Circular phase-space visualization

### Quant Monte Carlo

* Geometric Brownian Motion (GBM)
* Multithreaded Monte Carlo simulation
* Adjustable volatility
* Adjustable number of paths
* Adjustable number of simulation steps
* Adjustable thread count
* Sample path visualization
* Terminal price statistics

---

## Technologies

* C++20
* Qt5 Widgets
* Qt5 Charts
* SFML 2.6
* CMake
* Multithreading (std::thread)

---

## Project Structure

```text
src/
│
├── core/
│   ├── Simulation.hpp
│   └── SimulationManager.hpp
│
├── simulations/
│   ├── IsingSimulation
│   ├── PercolationSimulation
│   ├── KuramotoSimulation
│   └── QuantSimulation
│
├── gui/
│   ├── MainWindow
│   ├── ParameterPanel
│   └── PlotWidget
│
└── rendering/
    └── SFMLWidget
```

---

## Build Requirements

### Ubuntu / WSL

```bash
sudo apt update

sudo apt install \
    build-essential \
    cmake \
    git \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5charts5-dev \
    libsfml-dev
```

---

## Build Instructions

Clone repository:

```bash
git clone https://github.com/remiez/ComplexSystemSimulator.git
cd ComplexSystemSimulator
```

Create build directory:

```bash
mkdir build
cd build
```

Configure project:

```bash
cmake ..
```

Compile:

```bash
cmake --build . -j$(nproc)
```

Run:

```bash
./ComplexSystemSimulator
```

---

## Usage

1. Select a simulation from the drop-down menu.
2. Adjust simulation parameters using sliders.
3. Start, stop, or reset simulations.
4. Observe system evolution and statistics in real time.

---

## Future Development

### Ising

* Wolff cluster algorithm
* Swendsen-Wang algorithm
* Finite-size scaling
* Binder cumulants

### Percolation

* Union-Find implementation
* Hoshen-Kopelman algorithm
* Cluster size distributions

### Kuramoto

* Time evolution of synchronization order parameter
* Network topologies
* Frequency locking analysis

### Quant

* Option pricing
* Black-Scholes comparison
* Greeks estimation
* Monte Carlo variance reduction techniques
* Performance benchmarks

---

## Author

Michał Remiezowicz

Warsaw University of Technology

Faculty of Physics

---

## License

This project is intended for educational and research purposes.
