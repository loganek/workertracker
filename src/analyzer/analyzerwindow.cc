#include "analyzerwindow.h"

#include "wtcommon/filestorage.h"
#include "wtcommon/logger.h"

AnalyzerWindow::AnalyzerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
 : ApplicationWindow(cobject),
   builder(builder)
{
    add_action("search", [this] { on_search(); });
    add_action("export", [this] { on_export_current_view(); });
    add_action("load", [this] { on_load_log(); });

    load_widgets();

    load_model("/home/loganek/.workertracker/data.dat");

    search_entry->signal_changed().connect([this] { on_search_changed(); });
    search_entry->signal_stop_search().connect([this]{ on_search(); });

    filter_model->set_visible_func([this] (const Gtk::TreeModel::const_iterator& it) {
        return filter_func(it);
    });

    init_tree_view();

}

static std::string display(std::chrono::seconds sec)
{
    using namespace std::chrono;
    std::ostringstream ss;

    using namespace std::literals::chrono_literals;

    auto h = duration_cast<hours>(sec);
    if (h > 0h)
    {
        sec -= h;
        ss << h.count() << "h ";
    }
    auto m = duration_cast<minutes>(sec);
    if (m > 0min || h > 0h)
    {
        sec -= m;
        ss << m.count() << "min ";
    }
    ss << sec.count() << "s";
    return ss.str();
}

void AnalyzerWindow::load_widgets()
{
    builder->get_widget("search_entry", search_entry);
    builder->get_widget("treeView", tree_view);
    builder->get_widget("event_search", search_bar);
    builder->get_widget("header_bar", header_bar);
}

void AnalyzerWindow::on_search()
{
    bool state = !search_bar->get_search_mode();
    search_bar->set_search_mode(state);

    if (state)
    {
        search_entry->grab_focus();
        search_entry->set_position(-1);
    }
    else
    {
        search_entry->set_text("");
    }
}

void AnalyzerWindow::on_search_changed()
{
    try
    {
        filter_pattern = search_entry->get_text().empty() ? boost::none : boost::make_optional(std::regex((std::string)search_entry->get_text()));
    }
    catch (const std::regex_error &ex)
    {
        WT_LOG(WT::LogLevel::WARNING) << ex.what();
        return;
    }

    filter_model->refilter();

    update_parents_values();

    tree_view->expand_all();
}

void AnalyzerWindow::load_model(const std::string& filename)
{
    WT::FileStorage storage(filename);
    container.clear();
    storage.load_data(container);

    auto tree_model = Gtk::TreeStore::create(columns);
    filter_model = Gtk::TreeModelFilter::create(tree_model);

    for (auto app : container.get_keys())
    {
        auto row = *(tree_model->append());
        row[columns.m_col_name] = app;

        for (auto details : container.get_values(app))
        {
            auto childrow = *(tree_model->append(row.children()));
            childrow[columns.m_col_name] = details;
            childrow[columns.m_col_value] = container.get_duration(app, details).count();
            childrow[columns.m_col_value_str] = display(container.get_duration(app, details));
        }
    }

    update_parents_values();

    tree_view->set_model(filter_model);
    header_bar->set_subtitle(filename);
}

void AnalyzerWindow::update_parents_values()
{
    for (auto it : filter_model->children())
    {
        typename decltype(columns.m_col_value)::ElementType total = 0;

        for (auto child_it : it->children())
        {
            total += child_it->get_value(columns.m_col_value);
        }

        it->set_value(columns.m_col_value, total);
        it->set_value<Glib::ustring>(columns.m_col_value_str, display(std::chrono::seconds(total)));
    }
}

void AnalyzerWindow::init_tree_view()
{
    tree_view->append_column("Name", columns.m_col_name);
    tree_view->append_column("Time", columns.m_col_value_str);

    tree_view->expand_all();
}

bool AnalyzerWindow::filter_func(const Gtk::TreeModel::const_iterator& it)
{
    if (!it->parent())
    {
        return true;
    }
    else
    {
        std::string value = it->get_value(columns.m_col_name);
        return !filter_pattern || std::regex_search(value, filter_pattern.get());
    }
}

void AnalyzerWindow::on_export_current_view()
{
    Gtk::FileChooserDialog dialog(*this, "Save logs", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Save", Gtk::RESPONSE_ACCEPT);

    dialog.set_do_overwrite_confirmation(true);
    dialog.set_current_name("workertracker log.txt");

    if (dialog.run() == Gtk::RESPONSE_ACCEPT)
    {
        WT::DataContainer filtered_container;

        for (auto parent : filter_model->children())
        {
            for (auto child : parent.children())
            {
                filtered_container.insert(
                            (std::string)parent->get_value(columns.m_col_name),
                            (std::string)child->get_value(columns.m_col_name),
                            std::chrono::seconds(child->get_value(columns.m_col_value)));
            }
        }

        WT::FileStorage storage(dialog.get_filename());
        storage.save_data(filtered_container);
    }
}

void AnalyzerWindow::on_load_log()
{
    Gtk::FileChooserDialog dialog(*this, "Load log", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("OK", Gtk::RESPONSE_ACCEPT);

    if (dialog.run() == Gtk::RESPONSE_ACCEPT)
    {
        load_model(dialog.get_filename());
	tree_view->expand_all();
    }
}
