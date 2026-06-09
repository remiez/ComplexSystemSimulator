#pragma once

#include "../core/Parameter.hpp"
#include <QWidget>
#include <functional>
#include <vector>

class QVBoxLayout;

/** Dynamically builds slider/spin-box rows from Simulation parameter descriptors. */
class ParameterPanel : public QWidget{
    Q_OBJECT
    public:
    /** @param parent Optional Qt parent widget. */
    explicit ParameterPanel(QWidget* parent = nullptr);

    /**
     * Rebuild controls for the given parameters (clears previous rows).
     * @param parameters Current simulation parameters from getParameters().
     */
    void setParameters(const std::vector<Parameter>& parameters);

    /**
     * Register handler invoked when the user edits any parameter control.
     * @param callback Receives parameter name and new value.
     */
    void setOnParameterChanged(std::function<void(const std::string&,double)> callback);
    
    private:
    /** Remove and delete all widgets from the vertical layout. */
    void ClearLayout();

    QVBoxLayout* layout = nullptr;
    std::function<void(const std::string&,double)> OnParameterChanged;
    

};