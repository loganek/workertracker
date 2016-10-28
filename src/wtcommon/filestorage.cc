#include "filestorage.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace WT {

using boost::property_tree::ptree;

FileStorage::FileStorage(const std::string &filename)
    : filename(filename)
{
    create_file();
}

void FileStorage::create_file()
{
    if (!boost::filesystem::exists(filename))
    {
        DataContainer dt;
        save_data(dt);
    }
}

FileStorage::~FileStorage()
{
}

void FileStorage::load_data(DataContainer &container)
{
    container.clear();

    ptree pt;
    boost::property_tree::json_parser::read_json(filename, pt);

    for(auto& app : pt)
    {
        for(ptree::value_type& entry : pt.get_child(app.first.data()))
        {
            auto sec = std::stol(entry.second.data());
            container.insert(app.first.data(), entry.first.data(), std::chrono::seconds(sec));
        }
    }
}

static ptree::iterator push_value(ptree& tree, const ptree::value_type& val)
{
    ptree::iterator it;
    for (it = tree.begin(); it != tree.end(); ++it)
    {
        if (it->first == val.first)
        {
            it->second = val.second;
            break;
        }
    }

    if (it == tree.end())
    {
        it = tree.push_back(val);
    }

    return it;
}

void FileStorage::save_data(const DataContainer &container)
{
    ptree tree;

    for(const auto& app : container.get_keys())
    {
        ptree::iterator it = push_value(tree, ptree::value_type(app, ptree()));

        for(const auto& entry : container.get_values(app))
        {
            auto p= std::make_pair(entry, std::to_string(container.get_duration(app, entry).count()));
            push_value(it->second, ptree::value_type(p));
        }
    }

    boost::property_tree::json_parser::write_json(filename, tree);
}

}
