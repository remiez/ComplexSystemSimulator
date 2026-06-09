#include "QuantSimulation.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <thread>

std::string QuantSimulation::name() const
{
    return "Quant";
}

void QuantSimulation::reset()
{
    paths = std::max(1, paths);
    steps = std::max(1, steps);
    threads = std::max(1, threads);
    threads = std::min(threads, paths);

    int visiblePaths = std::min(displayedPaths, paths);

    finalPrices.assign(paths, 0.0);
    samplePaths.assign(visiblePaths, std::vector<double>(steps + 1, S0));

    meanFinalPrice = 0.0;
    elapsedMs = 0.0;
    simulationReady = false;

    runSimulation();
}

void QuantSimulation::update(double dt)
{
    time += dt;
}

void QuantSimulation::render(sf::RenderTarget& target)
{
    if(!simulationReady || samplePaths.empty())
    {
        return;
    }

    double minPrice = samplePaths[0][0];
    double maxPrice = samplePaths[0][0];

    for(const auto& path : samplePaths)
    {
        for(const double& price : path)
        {
            minPrice = std::min(minPrice, price);
            maxPrice = std::max(maxPrice, price);
        }
    }

    if(maxPrice - minPrice < 1e-9)
    {
        return;
    }

    auto size = target.getSize();

    float margin = 40.0f;
    float width = static_cast<float>(size.x) - 2.0f * margin;
    float height = static_cast<float>(size.y) - 2.0f * margin;

    for(const auto& path : samplePaths)
    {
        if(path.size() < 2)
        {
            continue;
        }

        sf::VertexArray line(sf::LineStrip, path.size());

        for(size_t i = 0; i < path.size(); ++i)
        {
            float x = margin + width * static_cast<float>(i)
                    / static_cast<float>(path.size() - 1);

            float normalized = static_cast<float>(
                (path[i] - minPrice) / (maxPrice - minPrice)
            );

            float y = margin + height * (1.0f - normalized);

            line[i].position = sf::Vector2f(x, y);
            line[i].color = sf::Color(100, 220, 120);
        }

        target.draw(line);
    }
}

std::vector<Parameter> QuantSimulation::getParameters() const
{
    return {
        {"Volatility", volatility, 0.01, 1.0, ParameterType::Double},
        {"Paths", static_cast<double>(paths), 1000.0, 200000.0, ParameterType::Integer},
        {"Threads", static_cast<double>(threads), 1.0, 16.0, ParameterType::Integer},
        {"Steps", static_cast<double>(steps), 50.0, 1000.0, ParameterType::Integer},
        {"Mu", mu, -2,2, ParameterType::Double}
    };
}

void QuantSimulation::setParameter(const std::string& name, double value)
{
    if(name == "Volatility")
    {
        volatility = value;
    }
    else if(name == "Paths")
    {
        paths = static_cast<int>(value);
    }
    else if(name == "Threads")
    {
        threads = static_cast<int>(value);
    }
    else if(name == "Steps")
    {
        steps = static_cast<int>(value);
    }
    else if(name == "Mu"){
        mu = value;
    }

}

std::vector<std::pair<std::string, double>> QuantSimulation::getStats() const
{
    return {
        {"Volatility", volatility},
        {"Paths", static_cast<double>(paths)},
        {"Steps", static_cast<double>(steps)},
        {"Threads", static_cast<double>(threads)},
        {"Mean S(T)", meanFinalPrice},
        {"Elapsed ms", elapsedMs},
        {"Mu", mu}
    };
}

void QuantSimulation::runSimulation()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> workers;
    workers.reserve(threads);

    int pathsPerThread = paths / threads;
    int remainder = paths % threads;

    int currentStart = 0;

    for(int t = 0; t < threads; ++t)
    {
        int extra = (t < remainder) ? 1 : 0;
        int currentEnd = currentStart + pathsPerThread + extra;

        unsigned int seed = static_cast<unsigned int>(
            std::chrono::high_resolution_clock::now()
                .time_since_epoch()
                .count()
        ) + static_cast<unsigned int>(t);

        workers.emplace_back(
            &QuantSimulation::simulateRange,
            this,
            currentStart,
            currentEnd,
            seed
        );

        currentStart = currentEnd;
    }

    for(auto& worker : workers)
    {
        worker.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

    computeMeanFinalPrice();
    simulationReady = true;
}

void QuantSimulation::simulateRange(int startPath, int endPath, unsigned int seed)
{
    std::mt19937 rng(seed);
    std::normal_distribution<double> normal(0.0, 1.0);

    double dt = T / static_cast<double>(steps);

    for(int p = startPath; p < endPath; ++p)
    {
        double S = S0;

        bool storePath = p < static_cast<int>(samplePaths.size());

        if(storePath)
        {
            samplePaths[p][0] = S0;
        }

        for(int step = 1; step <= steps; ++step)
        {
            double Z = normal(rng);

            S *= std::exp(
                (mu - 0.5 * volatility * volatility) * dt
                + volatility * std::sqrt(dt) * Z
            );

            if(storePath)
            {
                samplePaths[p][step] = S;
            }
        }

        finalPrices[p] = S;
    }
}

void QuantSimulation::computeMeanFinalPrice()
{
    if(finalPrices.empty())
    {
        meanFinalPrice = 0.0;
        return;
    }

    double sum = 0.0;

    for(const double& price : finalPrices)
    {
        sum += price;
    }

    meanFinalPrice = sum / static_cast<double>(finalPrices.size());
}