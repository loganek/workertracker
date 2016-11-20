#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <string>

class AnalyzerController;

class IMainWindow
{
protected:
    AnalyzerController *controller;

public:
    virtual ~IMainWindow() {}

    virtual void set_controller(AnalyzerController *controller) { this->controller = controller; }
    virtual void print_error(const std::string &error_msg) = 0;
    virtual void update_for_new_model() {}
};

#endif // IMAINWINDOW_H
