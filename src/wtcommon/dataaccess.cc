#include "dataaccess.h"

namespace WT {
std::unordered_map<std::string, operand_value_t> DataAccess::get_variables()
{
    static std::unordered_map<std::string, operand_value_t> var_map = {
        {"start", std::tm{}},
        {"end", std::tm{}},
        {"name", std::string{}},
        {"description", std::string{}}
    };

    return var_map;
}

}
