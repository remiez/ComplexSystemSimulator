#pragma once
#include <QWidget>
#include <vector>

/** Simple time-series line chart backed by external sample data. */
class PlotWidget : public QWidget
{
    Q_OBJECT
    public:
    /** @param parent Optional Qt parent widget. */
    explicit PlotWidget(QWidget* parent = nullptr);

    /**
     * Point at data to plot; caller must keep the vector alive.
     * @param newData Non-owning pointer, or nullptr to show empty state.
     */
    void setData(const std::vector<double>* newData);

    protected:
    /** Draw axes border and normalized polyline when at least two samples exist. */
    void paintEvent(QPaintEvent* event) override;

    private:
    const std::vector<double>* data = nullptr;
};