/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef DATAACCESS_H
#define DATAACCESS_H

#include "dataentry.h"
#include "datacontainer.h"
#include "binaryexpressiontree.h"

#include <ctime>
#include <string>
#include <vector>

namespace WT {

class DataAccess
{
public:
    virtual ~DataAccess() {}

    virtual void open(bool readonly) = 0;
    virtual void save_entry(const DataEntry &entry) = 0;
    virtual DataContainer get_tree(const std::shared_ptr<BinaryExpression>& expression) = 0; // TODO get rid of the method, container might convert data based on entries
    virtual DataContainerV2 get_entries(const std::shared_ptr<BinaryExpression>& expression) = 0;

    static std::unordered_map<std::string, operand_value_t> get_variables();
};

}

#endif // DATAACCESS_H
