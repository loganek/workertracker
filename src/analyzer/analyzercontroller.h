/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef ANALYZERCONTROLLER_H
#define ANALYZERCONTROLLER_H

#include "imainwindow.h"

#include "wtcommon/dataaccess.h"
#include "wtcommon/registrable.h"

#include <boost/optional.hpp>

#include <memory>

class AnalyzerController : public WT::RegistrableSingle<AnalyzerController>
{
    WT::DateRange period;

protected:
    boost::optional<std::regex> filter_pattern = boost::none;
    IMainWindow *main_window = nullptr;
    std::shared_ptr<WT::DataAccess> data_access;

    virtual void load_model(const WT::DataContainer &container) = 0;
    virtual IMainWindow* construct_window() = 0;

public:
    virtual ~AnalyzerController() {}

    virtual int run(int argc, char **argv);

    virtual void apply_filter(const std::string &search_text, bool case_sensitive);
    void load_from_file(const std::string &filename);

    void set_period(const WT::DateRange &period);
};

#endif // ANALYZERCONTROLLER_H
