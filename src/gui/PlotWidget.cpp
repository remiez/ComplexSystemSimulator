#include "PlotWidget.hpp"
#include <QPainter>
#include <QPainterPath>
#include <iostream>

PlotWidget::PlotWidget(QWidget* parent)
    :QWidget(parent)
{
    setMinimumHeight(200);
}

/** Store non-owning series pointer and request a repaint; caller keeps the vector alive. */
void PlotWidget::setData(const std::vector<double>* newData){
    data = newData;
    update();
}

/**
 * Draw a min–max normalized time series as a polyline.
 * Sample index maps to horizontal position; values are scaled to the widget height (y flipped for Qt coords).
 */
void PlotWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.fillRect(rect(), QColor(30,30,30));
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(rect().adjusted(1, 1, -2, -2));
    if(!data){
        painter.drawText(rect(),Qt::AlignCenter, "No data");
        return;
    }
    if(data->size() < 2){
        painter.drawText(rect(),Qt::AlignCenter, "Waiting For Data");
        return;
    }
    // Auto-scale to the current series range; epsilon avoids division when flat.
    double minValue = (*data)[0];
    double maxValue = (*data)[0];
    for(double v : *data){
        minValue = std::min(minValue,v);
        maxValue = std::max(maxValue,v);
    }
    QPainterPath path;
    for(size_t i= 0; i<data->size(); ++i){
        double x = static_cast<double>(i)/(data->size()-1);
        double y = ((*data)[i]-minValue)/(maxValue-minValue+1e-12);
        QPointF point(x*width(),height()*(1-y));

        if(i==0){
            path.moveTo(point);
        }
        else{
            path.lineTo(point);
        }

    }
    painter.setPen(Qt::white);
    painter.drawText(10, 20, QString("points: %1").arg(data->size()));
    painter.setPen(QPen(Qt::green, 2));
    painter.drawPath(path);
    
}