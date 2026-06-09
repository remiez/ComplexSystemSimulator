#include "KuramotoSimulation.hpp"

#include <cmath>
#include <random>
#include <numbers>
#include <algorithm>

std::string KuramotoSimulation::name() const{
    return "Kuramoto";
}

/** Randomize phases uniformly on [0, 2π) and natural frequencies in [−spread, spread]. */
void KuramotoSimulation::reset(){
    theta.resize(oscillators);
    omega.resize(oscillators);
    thetaNew.resize(oscillators);

    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<double> phaseDist(0, 2*PI);
    std::uniform_real_distribution<double> freqDist(-frequencySpread, frequencySpread);

    for(int i = 0; i<oscillators; ++i){
        theta[i] = phaseDist(rng);
        omega[i] = freqDist(rng);
        thetaNew[i] = theta[i];
    }
    time=0;
    computeOrderParameter();
}

/**
 * Kuramoto dynamics: dθ_i/dt = ω_i + (K/N) Σ_j sin(θ_j − θ_i).
 * Phases are wrapped back into [0, 2π) after each Euler step.
 */
void KuramotoSimulation::update(double dt){
    if(theta.size() != static_cast<size_t>(oscillators) || omega.size() != static_cast<size_t>(oscillators) || thetaNew.size() != static_cast<size_t>(oscillators)){
        reset();
    }
    for(int i = 0; i<oscillators; ++i){
        double sum =0;
        for(int j = 0; j< oscillators; ++j){
            sum+= std::sin(theta[j]-theta[i]);
        }
        double dtheta = omega[i] + coupling/oscillators*sum;
        thetaNew[i] = theta[i] +dtheta*dt;
    }
    theta = thetaNew;
    time+=dt;
    for(double& angle : theta){
        if(angle > 2*PI){
            angle -= 2*PI;
        }
        if(angle < 0){
            angle +=2*PI;
        }
    }
    computeOrderParameter();
}

/** Map each oscillator phase to a point on a circle centered in the render target. */
void KuramotoSimulation::render(sf::RenderTarget& target){
    if(theta.empty()){
        return;
    }
    auto size = target.getSize();

    float cx = size.x/2.0f;
    float cy = size.y/2.0f;
    float R = 0.4f*std::min(size.x,size.y);
    sf::CircleShape boundary;
    boundary.setRadius(R);
    boundary.setOrigin(R,R);
    boundary.setPosition(cx,cy);
    boundary.setFillColor(sf::Color::Transparent);
    boundary.setOutlineColor(sf::Color(80,80,80));
    boundary.setOutlineThickness(2.0f);
    target.draw(boundary);
    sf::CircleShape point;
    for(int i =0; i<oscillators; ++i){
        float x = cx +R*cos(theta[i]);
        float y = cy +R*sin(theta[i]);
        point.setRadius(4.0f);
        point.setOrigin(4.0f,4.0f);
        point.setPosition(x,y);
        point.setFillColor(sf::Color(100,180,255));
        target.draw(point);

    }
}
std::vector<Parameter> KuramotoSimulation::getParameters() const{
    return {{"Coupling", coupling, 0, 10,ParameterType::Double}, 
        {"Oscillators", static_cast<double>(oscillators), 10, 300,ParameterType::Integer},
        {"Frequency Spread", frequencySpread, 0,5, ParameterType::Double}};
}

/** Oscillators or Frequency Spread change vector sizes and require re-initialization. */
void KuramotoSimulation::setParameter(const std::string& name, double value){
    if(name == "Coupling"){
        coupling=value;
    }
    else if(name == "Oscillators"){
        oscillators = value;
        reset();
    }
    else if(name == "Frequency Spread"){
        frequencySpread = value;
        reset();
    }
}
std::vector<std::pair<std::string, double>> KuramotoSimulation::getStats() const{
    return {{"Coupling",coupling}, 
    {"Oscillators", static_cast<double>(oscillators)}, 
    {"Frequency Spread", frequencySpread},
    {"Order r", orderParameter},
    {"Time", time}};
}

/** Synchronization order parameter r = |⟨e^{iθ}⟩| (mean-field phase coherence). */
void KuramotoSimulation::computeOrderParameter(){
    double real = 0;
    double imag = 0;
    if(theta.empty()){
        orderParameter=0;
        return;
    }
    for(double& angle : theta){
        real+= std::cos(angle);
        imag+= std::sin(angle);
    }
    real /= theta.size();
    imag /= theta.size();

    orderParameter = std::sqrt(real*real+imag*imag);


}