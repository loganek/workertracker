#ifndef ANALYZERCONTROLLER_H
#define ANALYZERCONTROLLER_H

#include "imainwindow.h"

#include "wtcommon/dataaccess.h"
#include "wtcommon/registrable.h"

#include <boost/optional.hpp>

#include <memory>

class AnalyzerController : public WT::RegistrableSingle<AnalyzerController>
{
protected:
    boost::optional<std::regex> filter_pattern = boost::none;

    IMainWindow *main_window = nullptr;

    std::shared_ptr<WT::DataAccess> data_access;

    virtual void load_model_to_view(const WT::DataContainer &container) = 0;

    virtual void apply_filter() = 0;
    virtual IMainWindow* construct_window() = 0;

public:
    virtual ~AnalyzerController() {}

    virtual int run(int argc, char **argv);
    void on_search(const std::string &search_text, bool case_sensitive);
    void load_from_file(const std::string &filename);

    static std::time_t seconds_from_epoch(const std::string& s);
    static std::string time_to_display(std::chrono::seconds sec);
};

#endif // ANALYZERCONTROLLER_H
