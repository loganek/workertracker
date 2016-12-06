#include "wtcommon/mergealgorithm.h"

#include "gtest/gtest.h"

void assert_output(const std::vector<WT::DataEntry> &expected, const std::vector<WT::DataEntry> &output)
{
    ASSERT_EQ(expected.size(), output.size());

    for (std::size_t i = 0; i < expected.size(); i++)
    {
        ASSERT_EQ(expected[i].time_end, output[i].time_end);
        ASSERT_EQ(expected[i].time_start, output[i].time_start);
        ASSERT_EQ(expected[i].description, output[i].description);
        ASSERT_EQ(expected[i].proc_name, output[i].proc_name);
    }
}

TEST(MergeProcess, AdditionalNotInRange_ShouldNotIncludeIt)
{
    std::vector<WT::DataEntry> base = {
        WT::DataEntry(5, 17, "A", "")
    };

    std::vector<WT::DataEntry> additional = {
        WT::DataEntry(23, 47, "1", "")
    };

    std::vector<WT::DataEntry> output;
    WT::MergeAlgorithm::merge(base, additional, output);
    assert_output(base, output);
}

TEST(MergeProcess, AdditionalSurroundsRange_ShouldGetAdditional)
{
    std::vector<WT::DataEntry> base = {
        WT::DataEntry(5, 17, "A", "")
    };

    std::vector<WT::DataEntry> additional = {
        WT::DataEntry(3, 20, "1", "")
    };

    std::vector<WT::DataEntry> expected = {
        WT::DataEntry(5, 17, "1", "")
    };

    std::vector<WT::DataEntry> output;
    WT::MergeAlgorithm::merge(base, additional, output);
    assert_output(expected, output);
}

TEST(MergeProcess, AdditionalOverlapsBaseFromLeft)
{
    std::vector<WT::DataEntry> base = {
        WT::DataEntry(5, 17, "A", "")
    };

    std::vector<WT::DataEntry> additional = {
        WT::DataEntry(3, 12, "1", "")
    };

    std::vector<WT::DataEntry> expected = {
        WT::DataEntry(5, 12, "1", ""),
        WT::DataEntry(12, 17, "A", "")
    };

    std::vector<WT::DataEntry> output;
    WT::MergeAlgorithm::merge(base, additional, output);
    assert_output(expected, output);
}

TEST(MergeProcess, AdditionalOverlapsBaseFromRight)
{
    std::vector<WT::DataEntry> base = {
        WT::DataEntry(5, 17, "A", "")
    };

    std::vector<WT::DataEntry> additional = {
        WT::DataEntry(12, 22, "1", "")
    };

    std::vector<WT::DataEntry> expected = {
        WT::DataEntry(5, 12, "A", ""),
        WT::DataEntry(12, 17, "1", "")
    };

    std::vector<WT::DataEntry> output;
    WT::MergeAlgorithm::merge(base, additional, output);
    assert_output(expected, output);
}

TEST(MergeProcess, AdditionalInTheMiddleOfBase)
{
    std::vector<WT::DataEntry> base = {
        WT::DataEntry(5, 17, "A", "")
    };

    std::vector<WT::DataEntry> additional = {
        WT::DataEntry(12, 14, "1", "")
    };

    std::vector<WT::DataEntry> expected = {
        WT::DataEntry(5, 12, "A", ""),
        WT::DataEntry(12, 14, "1", ""),
        WT::DataEntry(14, 17, "A", "")
    };

    std::vector<WT::DataEntry> output;
    WT::MergeAlgorithm::merge(base, additional, output);
    assert_output(expected, output);
}

TEST(MergeProcess, MultipleEntries)
{
    std::vector<WT::DataEntry> base = {
        WT::DataEntry(5, 17, "A", ""),
        WT::DataEntry(25, 67, "B", ""),
        WT::DataEntry(90, 120, "C", ""),
        WT::DataEntry(150, 170, "D", ""),
    };

    std::vector<WT::DataEntry> additional = {
        WT::DataEntry(3, 7, "1", ""),
        WT::DataEntry(10, 19, "2", ""),
        WT::DataEntry(21, 23, "3", ""),
        WT::DataEntry(29, 56, "4", ""),
        WT::DataEntry(87, 112, "5", ""),
        WT::DataEntry(115, 200, "6", "")
    };

    std::vector<WT::DataEntry> expected = {
        WT::DataEntry(5, 7, "1", ""),
        WT::DataEntry(7, 10, "A", ""),
        WT::DataEntry(10, 17, "2", ""),
        WT::DataEntry(25, 29, "B", ""),
        WT::DataEntry(29, 56, "4", ""),
        WT::DataEntry(56, 67, "B", ""),
        WT::DataEntry(90, 112, "5", ""),
        WT::DataEntry(112, 115, "C", ""),
        WT::DataEntry(115, 120, "6", ""),
        WT::DataEntry(150, 170, "6", "")
    };

    std::vector<WT::DataEntry> output;
    WT::MergeAlgorithm::merge(base, additional, output);
    assert_output(expected, output);
}
