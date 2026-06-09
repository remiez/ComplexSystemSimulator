#include "MainWindow.hpp"

#include "ParameterPanel.hpp"
#include "PlotWidget.hpp"
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
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

#include <algorithm>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    connectSignals();
}

/** Build layout, wire parameter panel to the active simulation, and start the stats/update timer. */
void MainWindow::setupUi()
{
    updateTimer = new QTimer(this);
    // ~60 Hz tick: advance simulation when running, refresh Ising plot and stats label.
    connect(updateTimer, &QTimer::timeout, this, [this](){
        if(running){
            simulationManager.update(0.016);
        }
        auto* ising = dynamic_cast<IsingSimulation*>(simulationManager.currentSimulation());
        if(ising){
            magnetizationPlot->setData(&ising->getMagnetizationHistory());
            magnetizationPlot->update();
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
    // Forward spin-box edits to the owned simulation via SimulationManager.
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
    magnetizationPlot = new PlotWidget();
    magnetizationPlot->setMinimumHeight(200);
    startButton = new QPushButton("Start");
    pauseButton = new QPushButton("Pause");
    resetButton = new QPushButton("Reset");
    exportButton = new QPushButton("Export CSV");
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(resetButton);
    controlsLayout->addWidget(exportButton);

    statusLabel = new QLabel("Status: idle");
    statsLabel = new QLabel("Stats: -");
    statsLabel->setWordWrap(true);
    
    leftPanel->addWidget(statsLabel);
    leftPanel->addWidget(magnetizationPlot);
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
    // Default simulation: Ising, rendered in SFMLWidget and mirrored in ParameterPanel.
    simulationManager.setSimulation(std::make_unique<IsingSimulation>());
    renderWidget ->setSimulation(simulationManager.currentSimulation());
    parameterPanel -> setParameters(simulationManager.currentSimulation()->getParameters());
}

/** Connect run/pause/reset, CSV export, and simulation-type switching to SimulationManager and widgets. */
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

    connect(exportButton, &QPushButton::clicked,this, [this](){
        auto* ising = dynamic_cast<IsingSimulation*>(simulationManager.currentSimulation());

        if(!ising){
            QMessageBox::warning(this,"Export CSV", "Export for now only for ising simulation");
            return;
        }

        QString fileName =  QFileDialog::getSaveFileName(this,"Save Ising Data","ising_data.csv","CSV files (*.csv)");

        if(fileName.isEmpty()){
            return;
        }

        QFile file(fileName);

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::critical(this,"Export CSV", "Could not open file for writing");
            return;
        }

        QTextStream out(&file);

        const auto& magnetization = ising->getMagnetizationHistory();
        const auto& energy = ising->getEnergyHistory();
        const auto& acceptance = ising->getAcceptanceHistory();

        out<<"step,magnetization,energy_per_spin,acceptance_dation\n";

        const size_t n = std::min(magnetization.size(),std::min(energy.size(),acceptance.size()));

        for(size_t i = 0; i<n; ++i){
            out<<i<<","<<magnetization[i]<<","<<energy[i]<<","<<acceptance[i]<<"\n";
        }

        file.close();

        QMessageBox::information(this,"Export CSV", "Data exported successfully");
    });

    
    // Replace owned simulation, rebind SFMLWidget and rebuild parameter controls.
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