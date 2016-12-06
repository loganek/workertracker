#include "mergeprocess.h"

namespace WT {

MergeProcess::MergeProcess()
{

}

static DataEntry saturate(const DataEntry &in, const DataEntry &out)
{
    DataEntry output = out;
    if (out.time_start < in.time_start) output.time_start = in.time_start;
    if (out.time_end > in.time_end) output.time_end = in.time_end;
    return output;
}

std::vector<DataEntry> MergeProcess::merge(const std::vector<DataEntry> &base, const std::vector<DataEntry> &addition)
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
            output_entries.push_back(cur_base);
            continue;
        }

        if (ranges.front().time_start > cur_base.time_start)
        {
            output_entries.push_back(DataEntry(cur_base.time_start, ranges.front().time_start, cur_base.proc_name, cur_base.description));
        }

        output_entries.push_back(ranges[0]);

        for (std::size_t i = 1; i < ranges.size(); i++)
        {
            if (ranges[i].time_start > ranges[i-1].time_end)
            {
                output_entries.push_back(DataEntry(ranges[i-1].time_end, ranges[i].time_start, cur_base.proc_name, cur_base.description));
            }
            output_entries.push_back(ranges[i]);
        }

        if (ranges.back().time_end < cur_base.time_end)
        {
            output_entries.push_back(DataEntry(ranges.back().time_end, cur_base.time_end, cur_base.proc_name, cur_base.description));
        }
        else
        {
            i_addition--;
        }
    }

    return output_entries;
}

}
