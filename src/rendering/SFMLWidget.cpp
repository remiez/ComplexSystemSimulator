#include "SFMLWidget.hpp"
#include <QTimer>
#include <QShowEvent>
#include <QPaintEvent>
#include <QPainter>
#include "../core/Simulation.hpp"

SFMLWidget::SFMLWidget(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    renderTimer = new QTimer(this);

    connect(renderTimer, &QTimer::timeout,this,[this](){render();});
    renderTimer->start(16);

}
SFMLWidget::~SFMLWidget(){
    if (sfmlWindow.isOpen()){
        sfmlWindow.close();
    }
}

QPaintEngine* SFMLWidget::paintEngine() const{
    return nullptr;
}

void SFMLWidget::showEvent(QShowEvent* event){
    QWidget::showEvent(event);
    if (!initialized){
        initialize();
    }
}

void SFMLWidget::paintEvent(QPaintEvent* event){
    QWidget::paintEvent(event);

    render();
}

void SFMLWidget::initialize(){
    sfmlWindow.create((sf::WindowHandle)winId());
    sfmlWindow.setFramerateLimit(60);
    initialized = true;
}

void SFMLWidget::render(){
    if (!initialized){
        return;
    }
    sfmlWindow.clear(sf::Color(20,20,30));
    if (simulation)
    {
        simulation->render(sfmlWindow);
    }
    sfmlWindow.display();

}

void SFMLWidget::setSimulation(Simulation* sim){
    simulation=sim;
}