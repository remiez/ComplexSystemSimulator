#pragma once
#include <QMainWindow>
#include "../core/SimulationManager.hpp"

class QPushButton;
class QComboBox;
class QLabel;
class QTimer;

class SFMLWidget;
class ParameterPanel;
class PlotWidget;

/** Main application window: simulation selection, controls, stats, and live plots. */
class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
    /** @param parent Optional Qt parent widget. */
    explicit MainWindow(QWidget* parent = nullptr);
    
    private:
    /** Build widgets, layouts, and the ~16 ms update timer. */
    void setupUi();

    /** Wire buttons, combo box, parameter panel, and timer callbacks. */
    void connectSignals();
    
    QTimer* updateTimer = nullptr;
    bool running = false;
    QComboBox* simulationComboBox = nullptr;
    QLabel* statsLabel = nullptr;
    QPushButton* startButton = nullptr;
    QPushButton* pauseButton = nullptr;
    QPushButton* resetButton = nullptr;
    QPushButton* exportButton = nullptr;
    PlotWidget* magnetizationPlot = nullptr;
    QLabel* statusLabel = nullptr;

    ParameterPanel* parameterPanel = nullptr;
    SFMLWidget* renderWidget = nullptr;
    SimulationManager simulationManager;
    
};