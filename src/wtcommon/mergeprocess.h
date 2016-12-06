#ifndef MERGEPROCESS_H
#define MERGEPROCESS_H

#include "dataaccess.h"

namespace WT {

class MergeProcess
{
    std::vector<DataEntry> output_entries;
    std::shared_ptr<DataAccess> base_data_access;
    std::shared_ptr<DataAccess> addition_data_access;

public:
    MergeProcess::MergeProcess(const std::shared_ptr<DataAccess> &base_data_access, const std::shared_ptr<DataAccess> &addition_data_access);
    virtual ~MergeProcess() {}

    std::vector<DataEntry> run_merge(const std::vector<DataEntry> &base, const std::vector<DataEntry> &addition);
    std::vector<DataEntry> merge();
};

}

#endif // MERGEPROCESS_H
