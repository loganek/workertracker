#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <string>
#include <chrono>

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
    virtual void update_total_time(const std::chrono::seconds& seconds) = 0;
};

#endif // IMAINWINDOW_H
