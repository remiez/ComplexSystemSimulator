#pragma once
#include "../core/Simulation.hpp"
#include <random>
#include <vector>

/** Site percolation on a square lattice with cluster labeling and percolation detection. */
class PercolationSimulation : public Simulation
{
    public:
    PercolationSimulation();

    /** Implements Simulation::name(). @return Display name "Percolation". */
    std::string name() const override;

    /** Implements Simulation::reset(). Regenerates the grid and recomputes clusters. */
    void reset() override;

    /** Implements Simulation::update(). Advances elapsed time only; lattice is static between resets. */
    void update(double dt) override;

    /** Implements Simulation::render(). Colors cells by cluster role (percolating, largest, other). */
    void render(sf::RenderTarget& target) override;

    /** Implements Simulation::getParameters(). */
    std::vector<Parameter> getParameters() const override;

    /** Implements Simulation::setParameter(). Any change triggers reset(). */
    void setParameter(const std::string& name, double value) override;

    /** Implements Simulation::getStats(). Reports cluster count, largest size, and percolation flag. */
    std::vector<std::pair<std::string, double>> getStats() const override;

    private:
    /** Bernoulli occupation of each site at occupationPropability. */
    void generateGrid();

    /** Label connected occupied components via flood fill. */
    void identifyClusters();

    /** Breadth-first labeling of one occupied cluster. @param label Cluster index assigned to filled sites. */
    void floodFill(int startX, int startY, int label);

    /** Set doesPercolate if any cluster spans top-bottom or left-right boundaries. */
    void checkPercolation();

    /** Rebuild SFML quad vertices with cluster-based coloring. */
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
