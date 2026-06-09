#pragma once

#include <QWidget>
#include <SFML/Graphics.hpp>


class Simulation;
class QTimer;
/** Embeds an SFML render window inside a Qt widget for Simulation::render(). */
class SFMLWidget : public QWidget
{
    Q_OBJECT
    public:
    /** @param parent Optional Qt parent widget. */
    explicit SFMLWidget(QWidget* parent = nullptr);
    ~SFMLWidget() override;

    /**
     * Attach the simulation drawn each frame; not owned by this widget.
     * @param sim Active simulation, or nullptr to clear the view.
     */
    void setSimulation(Simulation* sim);
    
    protected:
    /** Disable Qt's paint engine so SFML owns the framebuffer. */
    QPaintEngine* paintEngine() const override;

    /** Lazily create the SFML window on first show. */
    void showEvent(QShowEvent* event) override;

    /** Trigger a render pass when Qt repaints the widget. */
    void paintEvent(QPaintEvent* event) override;

    private:
    /** Create sf::RenderWindow bound to this widget's native handle. */
    void initialize();

    /** Clear, delegate to Simulation::render(), and display. */
    void render();

    bool initialized = false;

    sf::RenderWindow sfmlWindow;
    QTimer* renderTimer = nullptr;
    Simulation* simulation = nullptr;
};

