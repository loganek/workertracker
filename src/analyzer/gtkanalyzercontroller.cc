#include "gtkanalyzercontroller.h"
#include "analyzerwindow.h"

int GtkAnalyzerController::run(int argc, char **argv)
{
    return !AnalyzerController::run(argc, argv) && app->run(*dynamic_cast<Gtk::Window*>(main_window));
}

void GtkAnalyzerController::init(int argc, char **argv)
{
    app = Gtk::Application::create(argc, argv, "eu.cookandcommit.workertracker.analyzer");

    app->signal_activate().connect([this] {
        app->set_accel_for_action("win.search", "<Primary>f");
    });

    filter_model = Gtk::TreeModelFilter::create(Gtk::TreeStore::create(columns));
    filter_model->set_visible_func([this] (const Gtk::TreeModel::const_iterator& it) {
        return filter_func(it);
    });
}

IMainWindow* GtkAnalyzerController::construct_window()
{
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/eu/cookandcommit/workertracker/analyzer/analyzer-window.glade");

    AnalyzerWindow *wnd;
    builder->get_widget_derived<AnalyzerWindow>("mainWindow", wnd);

    wnd->get_tree_view()->append_column("Name", columns.m_col_name);
    wnd->get_tree_view()->append_column("Time", columns.m_col_value_str);
    wnd->get_tree_view()->set_model(filter_model);

    return wnd;
}

bool GtkAnalyzerController::filter_func(const Gtk::TreeModel::const_iterator& it)
{
    if (!it->parent())
    {
        return true;
    }
    else
    {
        return !filter_pattern
                || std::regex_search(static_cast<std::string>(it->get_value(columns.m_col_name)), filter_pattern.get())
                || std::regex_search(static_cast<std::string>(it->parent()->get_value(columns.m_col_name)), filter_pattern.get());
    }
}

void GtkAnalyzerController::load_model_to_view(const WT::DataContainer &container)
{
    Glib::RefPtr<Gtk::TreeStore> tree_model = tree_model.cast_static(filter_model->get_model());\
    tree_model->clear();

    for (auto app : container.get_keys())
    {
        auto row = *(tree_model->append());
        row[columns.m_col_name] = app;

        for (auto details : container.get_values(app))
        {
            auto childrow = *(tree_model->append(row.children()));
            childrow[columns.m_col_name] = details;
            childrow[columns.m_col_value] = container.get_duration(app, details).count();
            childrow[columns.m_col_value_str] = AnalyzerController::time_to_display(container.get_duration(app, details));
        }
    }

    update_parents_values();

    dynamic_cast<AnalyzerWindow*>(main_window)->get_tree_view()->set_model(filter_model);
}


void GtkAnalyzerController::update_parents_values()
{
    for (auto it : filter_model->children())
    {
        typename decltype(columns.m_col_value)::ElementType total = 0;

        for (auto child_it : it->children())
        {
            total += child_it->get_value(columns.m_col_value);
        }

        it->set_value(columns.m_col_value, total);
        it->set_value<Glib::ustring>(columns.m_col_value_str, AnalyzerController::time_to_display(std::chrono::seconds(total)));
    }
}

void GtkAnalyzerController::apply_filter()
{
    filter_model->refilter();

    update_parents_values();
}

GtkAnalyzerController::RegistrarSingle<GtkAnalyzerController> GtkAnalyzerController::registrar;
