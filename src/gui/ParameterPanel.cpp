#include "ParameterPanel.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QSpinBox>


ParameterPanel::ParameterPanel(QWidget* parent) : QWidget(parent){
    layout = new QVBoxLayout(this);
}

void ParameterPanel::setOnParameterChanged(std::function<void(const std::string&,double)> callback){
    OnParameterChanged = std::move(callback);
}

void ParameterPanel::ClearLayout()
{
    while(auto* item = layout->takeAt(0)){
        if(auto* widget = item->widget()){
            widget->deleteLater();
        }

        delete item;
    }
}

void ParameterPanel::setParameters(const std::vector<Parameter>& parameters){
    ClearLayout();

    for(const auto& parameter : parameters){
        auto* row = new QWidget(this);
        auto* rowLayout = new QHBoxLayout(row);
        auto* label = new QLabel(QString::fromStdString(parameter.name), row);
        if(parameter.type == ParameterType::Double){
            auto* spinBox = new QDoubleSpinBox(row);
            
            spinBox ->setRange(parameter.minValue,parameter.maxValue);
            spinBox -> setValue(parameter.value);
            spinBox -> setDecimals(4); 

            rowLayout->addWidget(label);
            rowLayout->addWidget(spinBox);
            layout -> addWidget(row);
            connect(spinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,[this,parameter](double value)
                    {if (OnParameterChanged){
                            OnParameterChanged(parameter.name, value);
                        }
                    });
        }
        else{
            auto* spinBox = new QSpinBox(row);
            spinBox ->setRange(static_cast<int>(parameter.minValue),static_cast<int>(parameter.maxValue));
            spinBox -> setValue(static_cast<int>(parameter.value));

            rowLayout->addWidget(label);
            rowLayout->addWidget(spinBox);
            layout -> addWidget(row);
            connect(spinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,[this,parameter](int value)
                    {if (OnParameterChanged){
                            OnParameterChanged(parameter.name, static_cast<double>(value));
                        }
                    });
        }
        layout->addStretch();
    }
}