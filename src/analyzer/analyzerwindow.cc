/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "analyzerwindow.h"

#include "wtcommon/logger.h"

AnalyzerWindow::AnalyzerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
 : ApplicationWindow(cobject),
   builder(builder)
{
    add_action("search", [this] { on_search(); });
    add_action("export", [this] { on_export_current_view(); });
    add_action("load", [this] { on_load_from_file(); });

    load_widgets();

    search_entry->signal_changed().connect([this] { on_search_changed(); });
    search_entry->signal_stop_search().connect([this]{ on_search(); });
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
    controller->on_search(search_entry->get_text());
    tree_view->expand_all();
}

Gtk::TreeView* AnalyzerWindow::get_tree_view() const
{
    return tree_view;
}

void AnalyzerWindow::on_export_current_view()
{
    print_error("Not implemented yet.");
}

void AnalyzerWindow::on_load_from_file()
{
    Gtk::FileChooserDialog dialog(*this, "Load log", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("OK", Gtk::RESPONSE_ACCEPT);

    if (dialog.run() == Gtk::RESPONSE_ACCEPT)
    {
        controller->load_from_file(dialog.get_filename());
        tree_view->expand_all();
    }
}

void AnalyzerWindow::print_error(const std::string &error_msg)
{
    Gtk::MessageDialog dialog(*this, error_msg, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
    dialog.run();
}
