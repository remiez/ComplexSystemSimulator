#include "MainWindow.hpp"

#include "ParameterPanel.hpp"
#include "../rendering/SFMLWidget.hpp"
#include "../simulations/IsingSimulation.hpp"
#include "../simulations/PercolationSimulation.hpp"
#include "../simulations/KuramotoSimulation.hpp"
#include "../simulations/QuantSimulation.hpp"

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTimer>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    connectSignals();
}

void MainWindow::setupUi()
{
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this](){
        if(running){
            simulationManager.update(0.016);
        }
        if(simulationManager.currentSimulation()){
            QString text;

            for(const auto& stat: simulationManager.currentSimulation()->getStats()){
                text += QString::fromStdString(stat.first);
                text +=": ";
                text += QString::number(stat.second, 'f', 4);
                text += "\n";
            }

            statsLabel->setText(text);
        }
        });
    updateTimer->start(16);
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(centralWidget);
    auto* leftPanel = new QVBoxLayout();

    auto* simulationBox = new QGroupBox("Simulation");
    auto* simulationLayout = new QVBoxLayout(simulationBox);
    auto* parameterBox = new QGroupBox("Parameters");
    auto* parameterBoxLayout =new QVBoxLayout(parameterBox);
    parameterPanel = new ParameterPanel();
    parameterBoxLayout ->addWidget(parameterPanel);
    parameterPanel->setOnParameterChanged([this](const std::string& name, double value)
        {
            if(simulationManager.currentSimulation()){
                simulationManager.currentSimulation()->setParameter(name,value);
            }
        });


    simulationComboBox = new QComboBox();

    simulationComboBox->addItem("Ising");
    simulationComboBox->addItem("Percolation");
    simulationComboBox->addItem("Kuramoto");
    simulationComboBox->addItem("Quant Monte Carlo");

    simulationLayout->addWidget(simulationComboBox);

    auto* controlsBox = new QGroupBox("Controls");
    auto* controlsLayout = new QVBoxLayout(controlsBox);

    startButton = new QPushButton("Start");
    pauseButton = new QPushButton("Pause");
    resetButton = new QPushButton("Reset");

    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(resetButton);

    statusLabel = new QLabel("Status: idle");
    statsLabel = new QLabel("Stats: -");
    statsLabel->setWordWrap(true);
    leftPanel->addWidget(statsLabel);

    leftPanel->addWidget(simulationBox);
    leftPanel->addWidget(parameterBox);
    leftPanel->addWidget(controlsBox);
    leftPanel->addWidget(statusLabel);
    leftPanel->addStretch();

    renderWidget = new SFMLWidget();

    mainLayout->addLayout(leftPanel, 1);
    mainLayout->addWidget(renderWidget, 4);

    setCentralWidget(centralWidget);
    setWindowTitle("Complex System Simulator");
    simulationManager.setSimulation(std::make_unique<IsingSimulation>());
    renderWidget ->setSimulation(simulationManager.currentSimulation());
    parameterPanel -> setParameters(simulationManager.currentSimulation()->getParameters());
}

void MainWindow::connectSignals()
{
    connect(startButton, &QPushButton::clicked, this, [this]() {
        running = true;
        statusLabel->setText("Status: running");
    });

    connect(pauseButton, &QPushButton::clicked, this, [this]() {
        running = false;
        statusLabel->setText("Status: paused");
    });

    connect(resetButton, &QPushButton::clicked, this, [this]() {
        if(simulationManager.currentSimulation()){
            simulationManager.currentSimulation()->reset();
        }
        statusLabel->setText("Status: reset");
    });

    
    connect(simulationComboBox, &QComboBox::currentTextChanged, this, 
            [this](const QString& text) {
                if(text == "Ising"){
                    simulationManager.setSimulation(std::make_unique<IsingSimulation>());
                }
                else if(text == "Percolation"){
                    simulationManager.setSimulation(std::make_unique<PercolationSimulation>());
                }
                else if(text == "Kuramoto"){
                    simulationManager.setSimulation(std::make_unique<KuramotoSimulation>());
                }
                else if(text == "Quant Monte Carlo"){
                    simulationManager.setSimulation(std::make_unique<QuantSimulation>());
                }

                renderWidget->setSimulation(simulationManager.currentSimulation());
                statusLabel->setText("Status: " + text);
                
                if(simulationManager.currentSimulation()){
                    parameterPanel->setParameters(simulationManager.currentSimulation()->getParameters());
                }
            });

}