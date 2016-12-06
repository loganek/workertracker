#ifndef MERGEPROCESS_H
#define MERGEPROCESS_H

#include "dataaccess.h"

namespace WT {

class MergeProcess
{
    std::vector<DataEntry> output_entries;

public:
    MergeProcess();
    virtual ~MergeProcess() {}

    std::vector<DataEntry> merge(const std::vector<DataEntry> &base, const std::vector<DataEntry> &addition);
};

}

#endif // MERGEPROCESS_H
