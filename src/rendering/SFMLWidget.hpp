#pragma once

#include <QWidget>
#include <SFML/Graphics.hpp>


class Simulation;
class QTimer;
class SFMLWidget : public QWidget
{
    Q_OBJECT
    public:
    explicit SFMLWidget(QWidget* parent = nullptr);
    ~SFMLWidget() override;
    void setSimulation(Simulation* sim);
    
    protected:
    QPaintEngine* paintEngine() const override;
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    private:
    void initialize();
    void render();

    bool initialized = false;

    sf::RenderWindow sfmlWindow;
    QTimer* renderTimer = nullptr;
    Simulation* simulation = nullptr;
};

