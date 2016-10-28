#ifndef ANALYZERWINDOW_H
#define ANALYZERWINDOW_H

#include "wtcommon/datacontainer.h"

#include <gtkmm.h>

#include <boost/optional.hpp>

#include <string>
#include <regex>

class InfoModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:

    InfoModelColumns() { add(m_col_name); add(m_col_value); add(m_col_value_str); }

    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_value_str;
    Gtk::TreeModelColumn<long int> m_col_value;
};


class AnalyzerWindow : public Gtk::ApplicationWindow
{
    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::TreeView *tree_view;
    Gtk::SearchBar* search_bar;
    Gtk::SearchEntry *search_entry;
    Gtk::HeaderBar *header_bar;

    Glib::RefPtr<Gtk::TreeModelFilter> filter_model;

    InfoModelColumns columns;

    boost::optional<std::regex> filter_pattern = boost::none;

    WT::DataContainer container;

    void load_widgets();
    void load_model(const std::string &filename);
    void init_tree_view();
    bool filter_func(const Gtk::TreeModel::const_iterator& it);
    void on_search_changed();
    void update_parents_values();
    void on_search();
    void on_export_current_view();
    void on_load_log();

public:
    AnalyzerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~AnalyzerWindow() {}
};

#endif // ANALYZERWINDOW_H
