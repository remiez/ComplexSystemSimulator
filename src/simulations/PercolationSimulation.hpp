#pragma once
#include "../core/Simulation.hpp"
#include <random>
#include <vector>

class PercolationSimulation : public Simulation
{
    public:
    PercolationSimulation();
    std::string name() const override;
    void reset() override;
    void update(double dt) override;
    void render(sf::RenderTarget& target) override;
    std::vector<Parameter> getParameters() const override;
    void setParameter(const std::string& name, double value) override;
    std::vector<std::pair<std::string, double>> getStats() const override;

    private:
    void generateGrid();
    void identifyClusters();
    void floodFill(int startX, int startY, int label);
    void checkPercolation();
    void updateVertexArray(float cellWidth, float cellHeight);

    int index(int x, int y) const;
    bool isInside(int x, int y) const;

    double occupationPropability = 0.5; 
    double gridSize = 128;
    std::vector<int> grid;
    std::vector<int> clusterLabels;
    int clusterCount = 0;
    int percolatingClusterLabel = -1;
    int largestClusterLabel = -1;
    int largestClusterSize = 0;
    bool doesPercolate = false;
    double time = 0;
    std::mt19937 rng;
    std::uniform_real_distribution<double> uniform;
    sf::VertexArray vertices;   
};