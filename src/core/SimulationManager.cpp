#include "SimulationManager.hpp"
#include "Simulation.hpp"

/** Take ownership of sim; replaces any previously loaded simulation. */
void SimulationManager::setSimulation(std::unique_ptr<Simulation> sim){
    simulation = std::move(sim);
}

/** Non-owning view of the active simulation, valid until the next setSimulation(). */
Simulation* SimulationManager::currentSimulation(){
    return simulation.get();
}

/** Forward dt to the owned simulation when one is loaded. */
void SimulationManager::update(double dt){
    if (simulation){
        simulation->update(dt);
    }
}
