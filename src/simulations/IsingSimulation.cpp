#include "IsingSimulation.hpp"
#include <algorithm>
#include <cmath>

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
    attemptedFlips=0;
    acceptedFlips=0;
    acceptanceRatioHistory.clear();
    energyHistory.clear();
    magnetiztionHistory.clear();
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
    attemptedFlips++;
    std::uniform_int_distribution<int> siteDist(0, latticeSize-1);
    int x = siteDist(rng);
    int y = siteDist(rng);

    int s = spin(x,y);

    int neighborSum = spin(x+1,y)+spin(x-1,y)+spin(x,y-1)+spin(x,y+1);
    double deltaE = 2*s*(couplingJ*neighborSum+magneticField);
    if (deltaE <= 0){
        spins[y*latticeSize+x]*=-1;
        acceptedFlips++;
    }
    else{
        double p = std::exp(-deltaE/temperature);
        if(uniform(rng) < p){
            spins[y*latticeSize+x]*=-1;
        }
    }

}

void IsingSimulation::trimHistory(std::vector<double>& history){
    if( history.size() > 500){
        history.erase(history.begin());
    }
}

void IsingSimulation::update(double dt){
    for(int i=0;i < stepsPerFrame; ++i){
        metropolisStep();
    }
    double m = ComputeMagnetization();
    magnetiztionHistory.push_back(m);
    trimHistory(magnetiztionHistory);

    double E = ComputeEnergy()/static_cast<double>(spins.size());
    energyHistory.push_back(m);
    trimHistory(energyHistory);

    double acceptanceRatio = attemptedFlips>0 ? static_cast<double>(acceptedFlips)/static_cast<double>(attemptedFlips) : 0;
    acceptanceRatioHistory.push_back(acceptanceRatio);
    trimHistory(acceptanceRatioHistory);
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
    return {{"Temperature", temperature, 0.1, 5.0,ParameterType::Double}, 
        {"Magnetic Field",magneticField, -2, 2,ParameterType::Double}, 
        {"Steps Per Frame",static_cast<double>(stepsPerFrame),1,100000,ParameterType::Integer},
        {"Lattice Size", static_cast<double>(latticeSize),16,512,ParameterType::Integer}};
}

void IsingSimulation::setParameter(const std::string& name, double value){
    if(name == "Temperature"){
        temperature = value;
    }
    else if(name == "Magnetic Field"){
        magneticField = value;
    }
    else if(name == "Steps Per Frame"){
        stepsPerFrame = static_cast<int>(value);
    }
    else if(name == "Lattice Size"){
        latticeSize = value;
        reset();
    }
}

std::vector<std::pair<std::string, double>> IsingSimulation::getStats() const{
    double acceptanceRatio = attemptedFlips > 0 ? static_cast<double>(acceptedFlips)/static_cast<double>(attemptedFlips) : 0;
    double energyPerSpin = ComputeEnergy()/static_cast<double>(spins.size());
    return {{"Temperature", temperature}, 
        {"Magnetic Field", magneticField}, 
        {"Energy",ComputeEnergy()},
        {"MC sweeps", static_cast<double>(MonteCarloSteps)},
        {"Steps Per Frame", static_cast<int>(stepsPerFrame)}, 
        {"Lattice Size", latticeSize},
        {"Magnetization", ComputeMagnetization()},
        {"Abs Magnetization", std::abs(ComputeMagnetization())},
        {"Energy Per Spin", energyPerSpin},
        {"Acceptance Ratio", acceptanceRatio}};

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
const std::vector<double>& IsingSimulation::getMagnetizationHistory() const{
    return magnetiztionHistory;
}
const std::vector<double>& IsingSimulation::getEnergyHistory() const{
    return energyHistory;
}
const std::vector<double>& IsingSimulation::getAcceptanceHistory() const{
    return acceptanceRatioHistory;
}