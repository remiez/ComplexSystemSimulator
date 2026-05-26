#include "SimulationManager.hpp"
#include "Simulation.hpp"

void SimulationManager::setSimulation(std::unique_ptr<Simulation> sim){
    simulation = std::move(sim);
}

Simulation* SimulationManager::currentSimulation(){
    return simulation.get();
}
void SimulationManager::update(double dt){
    if (simulation){
        simulation->update(dt);
    }
}
