/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef GTKANALYZERCONTROLLER_H
#define GTKANALYZERCONTROLLER_H

#include "analyzercontroller.h"

#include <gtkmm.h>

class InfoModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:

    InfoModelColumns() { add(m_col_name); add(m_col_value); add(m_col_value_str); }

    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_value_str;
    Gtk::TreeModelColumn<long int> m_col_value;
};

class GtkAnalyzerController : public AnalyzerController
{
    static RegistrarSingle<GtkAnalyzerController> registrar;

    Glib::RefPtr<Gtk::Application> app;

    InfoModelColumns columns;
    Glib::RefPtr<Gtk::TreeModelFilter> filter_model;

    bool filter_func(const Gtk::TreeModel::const_iterator& it);
    void update_parents_values();

    void load_model_to_view(const WT::DataContainer &container) override;
    void apply_filter() override;
    IMainWindow* construct_window() override;

public:
    int run(int argc, char **argv) override;
};

#endif // GTKANALYZERCONTROLLER_H
