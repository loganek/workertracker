#include "mergeprocess.h"
#include "mergealgorithm.h"

namespace WT {

MergeProcess::MergeProcess(const std::shared_ptr<DataAccess> &base_data_access, const std::shared_ptr<DataAccess> &addition_data_access)
    : base_data_access(base_data_access), addition_data_access(addition_data_access)
{
}

std::vector<DataEntry> MergeProcess::run_merge(const std::vector<DataEntry> &base, const std::vector<DataEntry> &addition)
{
    MergeAlgorithm::merge(base, addition, output_entries);
    return output_entries;
}

std::vector<DataEntry> MergeProcess::merge()
{
    data_access->get_entries(base_data_access->get_entries(), addition_data_access->get_entries())
}

}
