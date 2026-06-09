#include "PercolationSimulation.hpp"
#include <queue>
PercolationSimulation::PercolationSimulation()
    :rng(std::random_device{}()), uniform(0,1)
{
    reset();
}
std::string PercolationSimulation::name() const{
    return "Percolation";
}

void PercolationSimulation::reset(){
    grid.resize(gridSize*gridSize);
    clusterLabels.resize(gridSize*gridSize);

    generateGrid();
    identifyClusters();
    checkPercolation();

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(gridSize*gridSize*4);
}
void PercolationSimulation::update(double dt){
    time+=dt;
}
void PercolationSimulation::render(sf::RenderTarget& target){
    auto size = target.getSize();
    float cellWidth = static_cast<float>(size.x)/static_cast<float>(gridSize);
    float cellHeight = static_cast<float>(size.y)/static_cast<float>(gridSize);
    updateVertexArray(cellWidth,cellHeight);
    target.draw(vertices);

}
std::vector<Parameter> PercolationSimulation::getParameters() const{
    return {{"Propability",occupationPropability,0,1,ParameterType::Double},
        {"Grid Size", static_cast<double>(gridSize), 16,512, ParameterType::Integer}};
}
/** Any parameter change regenerates the random grid and recomputes clusters. */
void PercolationSimulation::setParameter(const std::string& name, double value){
    if(name == "Propability"){
        occupationPropability = value;
        reset();
    }
    else if(name == "Grid Size"){
        gridSize = static_cast<int>(value);
        reset();
    }
}
std::vector<std::pair<std::string, double>> PercolationSimulation::getStats() const{
    return {{"Propability", occupationPropability}, 
        {"Grid Size", static_cast<double>(gridSize)}, 
        {"Clusters",static_cast<double>(clusterCount)},
        {"Largest Cluster Size", static_cast<double>(largestClusterSize)},
        {"Percolates", doesPercolate ? 1 : 0}};
}

/** Bernoulli site occupation: each cell is occupied independently with probability p. */
void PercolationSimulation::generateGrid(){
    for(auto& cell : grid){
        cell = (uniform(rng) < occupationPropability) ? 1 : 0;
    }
}
/** Label each occupied connected component; assign clusterCount and track the largest cluster. */
void PercolationSimulation::identifyClusters(){
    std::fill(clusterLabels.begin(),clusterLabels.end(), -1);
    clusterCount = 0;
    largestClusterLabel = -1;
    largestClusterSize = 0;
    for(int x = 0; x<gridSize; ++x){
        for(int y = 0; y<gridSize; ++y){
            if(grid[index(x,y)] == 1 && clusterLabels[index(x,y)] == -1){
                floodFill(x,y,clusterCount);
                clusterCount++;
            }
        }
    }
}
/** Breadth-first search labeling of one 4-connected occupied cluster starting at (startX, startY). */
void PercolationSimulation::floodFill(int startX, int startY, int label){
    int clusterSize = 0;
    std::queue<std::pair<int,int>> q;
    q.push({startX,startY});
    clusterLabels[index(startX,startY)] = label;
    while(!q.empty()){
        auto [x,y] = q.front();
        q.pop();
        clusterSize++;
        const int dx[4] = {1,-1,0,0};
        const int dy[4] = {0,0,1,-1};
        for(int k = 0; k<4; ++k){
            int nx = x+ dx[k];
            int ny = y+ dy[k];
            if(!isInside(nx,ny)){
                continue;
            }
            int id = index(nx,ny);
            if(grid[id] ==1 && clusterLabels[id] == -1){
                clusterLabels[id] = label;
                q.push({nx,ny});
            }
        }
    }
    if(clusterSize> largestClusterSize){
        largestClusterSize = clusterSize;
        largestClusterLabel = label;
    }
}
/**
 * Percolation occurs when any cluster touches opposite boundaries:
 * top–bottom or left–right (open boundary percolation on the grid).
 */
void PercolationSimulation::checkPercolation(){
    doesPercolate = false;
    percolatingClusterLabel = -1;
    std::vector<bool> touchesTop(clusterCount, false);
    std::vector<bool> touchesBottom(clusterCount, false);
    std::vector<bool> touchesRight(clusterCount, false);
    std::vector<bool> touchesLeft(clusterCount, false);
    for(int x = 0; x<gridSize; ++x){
        for(int y =0; y<gridSize; ++y){
            int label=clusterLabels[index(x,y)];
            if(label >= 0){
                if(y == 0) touchesTop[label] = true;
                if(y == gridSize-1) touchesBottom[label] = true;
                if(x == 0) touchesLeft[label] = true;
                if(x == gridSize-1) touchesRight[label] = true;
            }
        }
    }
    for(int i =0; i<clusterCount; ++i){
        if(touchesTop[i] && touchesBottom[i] || touchesLeft[i] && touchesRight[i]){
            doesPercolate = true;
            percolatingClusterLabel = i;
            return;
        }
    }
}
/** Color cells by role: empty, percolating cluster, largest cluster, or other occupied sites. */
void PercolationSimulation::updateVertexArray(float cellWidth, float cellHeight){
    for(int y = 0;y<gridSize; ++y){
        for(int x = 0;x<gridSize; ++x){
            int id = index(x,y);
            int vertexIndex = id*4;
            float px = x*cellWidth;
            float py = y*cellHeight;
            vertices[vertexIndex+0].position = {px,py};
            vertices[vertexIndex+1].position = {px+cellWidth,py};
            vertices[vertexIndex+2].position = {px+cellWidth,py+cellHeight};
            vertices[vertexIndex+3].position = {px,py+cellHeight};
            sf::Color color;
            if(grid[id] == 0)
            {
                color = sf::Color(25,25,30);
            }
            else if(clusterLabels[id] == percolatingClusterLabel){
                color = sf::Color(90,220,120);
            }
            else if(clusterLabels[id] == largestClusterLabel){
                color = sf::Color(230,120,70);
            }
            else{
                color = sf::Color(120,140,180);
            }
            vertices[vertexIndex+0].color = color;
            vertices[vertexIndex+1].color = color;
            vertices[vertexIndex+2].color = color;
            vertices[vertexIndex+3].color = color;
        }
    }
}

int PercolationSimulation::index(int x, int y) const{
    return y*gridSize + x;
}

bool PercolationSimulation::isInside(int x, int y) const{
    return x>= 0 && x<gridSize &&y >=0 && y<gridSize;
}   