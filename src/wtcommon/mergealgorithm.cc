#include "mergealgorithm.h"

namespace WT {

static DataEntry saturate(const DataEntry &base, const DataEntry &addition)
{
    DataEntry output = addition;
    if (output.time_start < base.time_start) output.time_start = base.time_start;
    if (output.time_end > base.time_end) output.time_end = base.time_end;
    return output;
}

void MergeAlgorithm::merge(const collection_t& base, const collection_t& addition, collection_t& output)
{
    std::size_t i_base = 0, i_addition = 0;

    for (; i_base < base.size(); i_base++)
    {
        const auto& cur_base = base[i_base];
        std::vector<DataEntry> ranges;

        for (; i_addition < addition.size(); i_addition++)
        {
            auto new_add = saturate(cur_base, addition[i_addition]);
            if (new_add.time_start < new_add.time_end)
            {
                ranges.push_back(new_add);
            }
            else if (!ranges.empty())
            {
                break;
            }
        }

        if (ranges.empty())
        {
            output.push_back(cur_base);
            continue;
        }

        if (ranges.front().time_start > cur_base.time_start)
        {
            output.push_back(DataEntry(cur_base.time_start, ranges.front().time_start, cur_base.proc_name, cur_base.description));
        }

        output.push_back(ranges[0]);

        for (std::size_t i = 1; i < ranges.size(); i++)
        {
            if (ranges[i].time_start > ranges[i-1].time_end)
            {
                output.push_back(DataEntry(ranges[i-1].time_end, ranges[i].time_start, cur_base.proc_name, cur_base.description));
            }
            output.push_back(ranges[i]);
        }

        if (ranges.back().time_end < cur_base.time_end)
        {
            output.push_back(DataEntry(ranges.back().time_end, cur_base.time_end, cur_base.proc_name, cur_base.description));
        }
        else
        {
            i_addition--;
        }
    }
}

}
