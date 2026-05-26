#pragma once
#include <QMainWindow>
#include "../core/SimulationManager.hpp"

class QPushButton;
class QComboBox;
class QLabel;

class SFMLWidget;

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
    explicit MainWindow(QWidget* parent = nullptr);
    
    private:
    void setupUi();
    void connectSignals();
    
    QComboBox* simulationComboBox = nullptr;

    QPushButton* startButton = nullptr;
    QPushButton* pauseButton = nullptr;
    QPushButton* resetButton = nullptr;

    QLabel* statusLabel = nullptr;

    SFMLWidget* renderWidget = nullptr;
    SimulationManager simulationManager;
    
};