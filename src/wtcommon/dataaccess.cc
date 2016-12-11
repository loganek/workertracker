#include "dataaccess.h"

namespace WT {
std::unordered_map<std::string, operand_value_t> DataAccess::get_variables()
{
    static std::unordered_map<std::string, operand_value_t> var_map = {
        {"time", std::tm{}},
        {"weekday", int{}},
        {"hour", int{}},
        {"start", std::tm{}},
        {"start_weekday", int{}},
        {"start_hour", int{}},
        {"end", std::tm{}},
        {"end_weekday", int{}},
        {"end_hour", int{}},
        {"name", std::string{}},
        {"description", std::string{}}
    };

    return var_map;
}

}
