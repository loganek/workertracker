#include "analyzerwindow.h"
#include "wtcommon/logger.h"

#include <gtkmm.h>

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create(argc, argv, "eu.cookandcommit.workertracker.analyzer");
    app->signal_activate().connect([app] {
        app->set_accel_for_action("win.search", "<Primary>f");
    });

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/eu/cookandcommit/workertracker/analyzer/analyzer-window.glade");

    AnalyzerWindow* wnd;
    builder->get_widget_derived<AnalyzerWindow>("mainWindow", wnd);
    WT::MethodOutput::set_method(WT::StdOutput::output);

    return app->run(*wnd);
}
