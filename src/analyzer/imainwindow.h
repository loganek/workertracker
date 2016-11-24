/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
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
    virtual void show_window() = 0;
};

#endif // IMAINWINDOW_H
