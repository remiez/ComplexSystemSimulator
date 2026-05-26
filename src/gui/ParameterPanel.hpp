#pragma once

#include "../core/Parameter.hpp"
#include <QWidget>
#include <functional>
#include <vector>

class QVBoxLayout;

class ParameterPanel : public QWidget{
    Q_OBJECT
    public:
    explicit ParameterPanel(QWidget* parent = nullptr);
    void setParameters(const std::vector<Parameter>& parameters);
    void setOnParameterChanged(std::function<void(const std::string&,double)> callback);
    
    private:
    void ClearLayout();

    QVBoxLayout* layout = nullptr;
    std::function<void(const std::string&,double)> OnParameterChanged;
    

};