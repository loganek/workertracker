#ifndef ANALYZERWINDOW_H
#define ANALYZERWINDOW_H

#include "imainwindow.h"
#include "analyzercontroller.h"

#include <gtkmm.h>

class AnalyzerWindow : public Gtk::ApplicationWindow, public IMainWindow
{
    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::TreeView *tree_view;
    Gtk::SearchBar* search_bar;
    Gtk::SearchEntry *search_entry;
    Gtk::HeaderBar *header_bar;

    void load_widgets();
    void on_search_changed();
    void on_search();
    void on_export_current_view();
    void on_load_from_file();

    void print_error(const std::string &error_msg) override;

public:
    AnalyzerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~AnalyzerWindow() {}

    Gtk::TreeView *get_tree_view() const;
};

#endif // ANALYZERWINDOW_H
