#pragma once
#include <QMainWindow>
#include "../core/SimulationManager.hpp"

class QPushButton;
class QComboBox;
class QLabel;
class QTimer;

class SFMLWidget;
class ParameterPanel;

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
    explicit MainWindow(QWidget* parent = nullptr);
    
    private:
    void setupUi();
    void connectSignals();
    
    QTimer* updateTimer = nullptr;
    bool running = false;
    QComboBox* simulationComboBox = nullptr;

    QPushButton* startButton = nullptr;
    QPushButton* pauseButton = nullptr;
    QPushButton* resetButton = nullptr;

    QLabel* statusLabel = nullptr;

    ParameterPanel* parameterPanel = nullptr;
    SFMLWidget* renderWidget = nullptr;
    SimulationManager simulationManager;
    
};