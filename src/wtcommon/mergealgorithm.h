#ifndef MERGEALGORITHM_H
#define MERGEALGORITHM_H

#include "dataaccess.h"

namespace WT {

class MergeAlgorithm
{
public:
    typedef std::vector<DataEntry> collection_t;

    static void merge(const collection_t& base, const collection_t& addition, collection_t& output);
};

}

#endif // MERGEALGORITHM_H
