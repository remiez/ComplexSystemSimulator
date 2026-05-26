#include "IsingSimulation.hpp"
#include <algorithm>

IsingSimulation::IsingSimulation()
    :rng(std::random_device{}()), uniform(0,1)
{
    reset();
}
std::string IsingSimulation::name() const{
    return "Ising";
}

void IsingSimulation::reset(){
    spins.resize(latticeSize*latticeSize);
    randomizeSpin();
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(latticeSize*latticeSize*4);
    MonteCarloSteps = 0;
}

void IsingSimulation::randomizeSpin(){
    for(auto& s : spins){
        s = (uniform(rng) < 0.5) ? -1 : 1;
    }
}

int IsingSimulation::spin(int x, int y) const{
    x = (x+latticeSize) %latticeSize;
    y = (y+latticeSize) %latticeSize;
    return spins[y*latticeSize+x];
}

void IsingSimulation::metropolisStep(){
    std::uniform_int_distribution<int> siteDist(0, latticeSize-1);
    int x = siteDist(rng);
    int y = siteDist(rng);

    int s = spin(x,y);

    int neighborSum = spin(x+1,y)+spin(x-1,y)+spin(x,y-1)+spin(x,y+1);
    double deltaE = 2*s*(couplingJ*neighborSum+magneticField);
    if (deltaE <= 0){
        spins[y*latticeSize+x]*=-1;
    }
    else{
        double p = std::exp(-deltaE/temperature);
        if(uniform(rng) < p){
            spins[y*latticeSize+x]*=-1;
        }
    }
}

void IsingSimulation::update(double dt){
    double m = ComputeMagnetization();
    if( magnetiztionHistory.size() > maxHistorySize){
        magnetiztionHistory.erase(magnetiztionHistory.begin());
    }
    int stepsPerFrame = latticeSize * latticeSize;
    for(int i=0;i < stepsPerFrame; ++i){
        metropolisStep();

    }
    MonteCarloSteps++;
}

void IsingSimulation::render(sf::RenderTarget& target){
    auto size = target.getSize();

    float cellWidth = static_cast<float>(size.x)/latticeSize;
    float cellHeight = static_cast<float>(size.y)/latticeSize;
    float cellSize = std::min(cellWidth,cellHeight);

    updateVertexArray(cellSize);
    target.draw(vertices);
}
double IsingSimulation::ComputeMagnetization() const{
    double sum =0;
    for(int s : spins){
        sum+=s;
    }
    return sum/static_cast<double>(spins.size());
}

double IsingSimulation::ComputeEnergy() const{
    double E = 0.0;
    for(int y = 0; y <latticeSize; ++y){
        for(int x = 0; x <latticeSize; ++x){
            int s = spin(x,y);
            E -= couplingJ*s*(spin(x+1,y)+spin(x,y+1));
            E -= magneticField*s;
        }
    }
    return E;
}

std::vector<Parameter> IsingSimulation::getParameters() const{
    return {{"Temperature", temperature, 0.1, 5.0}, {"Magnetic Field",magneticField, -2, 2}};
}

void IsingSimulation::setParameter(const std::string& name, double value){
    if(name == "Temperature"){
        temperature = value;
    }
    else if(name == "Magnetic Field"){
        magneticField = value;
    }
}

std::vector<std::pair<std::string, double>> IsingSimulation::getStats() const{
    return {{"Temperature", temperature}, {"Magnetic Field", magneticField}, {"Energy",ComputeEnergy()},{"MC sweeps", static_cast<double>(MonteCarloSteps)}};

}

void IsingSimulation::updateVertexArray(float cellSize){

    for(int y = 0; y <latticeSize; ++y){
        for(int x = 0; x <latticeSize; ++x){
            int index = y*latticeSize + x;
            int vertexIndex = index *4;
            float px = x*cellSize;
            float py = y*cellSize;

            vertices[vertexIndex + 0].position = sf::Vector2f(px,py);
            vertices[vertexIndex + 1].position = sf::Vector2f(px +cellSize,py);
            vertices[vertexIndex + 2].position = sf::Vector2f(px+cellSize,py+cellSize);
            vertices[vertexIndex + 3].position = sf::Vector2f(px,py+cellSize);
            sf::Color color = spins[index]>0 ? sf::Color(230,80,80) : sf::Color(80,120,230);

            vertices[vertexIndex+0].color = color;
            vertices[vertexIndex+1].color = color;
            vertices[vertexIndex+2].color = color;
            vertices[vertexIndex+3].color = color;

        }
    }
}