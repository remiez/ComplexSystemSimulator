#pragma once
#include <QWidget>
#include <vector>

class PlotWidget : public QWidget
{
    Q_OBJECT
    public:
    explicit PlotWidget(QWidget* parent = nullptr);
    void setData(const std::vector<double>* newData);

    protected:
    void paintEvent(QPaintEvent* event) override;

    private:
    const std::vector<double>* data = nullptr;
};