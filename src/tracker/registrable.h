#ifndef REGISTRABLE_H
#define REGISTRABLE_H

// Automatically create collection of instances of derived classes
// Blog post: http://www.cookandcommit.eu/2016/10/automatic-collection-of-instances-of.html

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace WT {

template <typename Derived, typename Base>
struct RegistrarCollection
{
    template<typename ...Args>
    RegistrarCollection(Args&&... args)
    {
        Base::registry()[typeid(Derived)] =
                std::make_shared<Derived>(std::forward<Args>(args)...);
    }
};

template <typename Derived, typename Base>
struct RegistrarSingle
{
    template<typename ...Args>
    RegistrarSingle(Args&&... args)
    {
        Base::registry() =
                std::make_shared<Derived>(std::forward<Args>(args)...);
    }
};

template<typename T>
struct RegistrableCollection
{
    typedef std::unordered_map<std::type_index, std::shared_ptr<T>> collection_t;
    template<typename R>
    using RegistrarCollection = RegistrarCollection<R, T>;

    static collection_t & registry()
    {
        static collection_t collection;
        return collection;
    }
};

template<typename T>
struct RegistrableSingle
{
    template<typename R>
    using RegistrarSingle = RegistrarSingle<R, T>;

    static std::shared_ptr<T> & registry()
    {
        static std::shared_ptr<T> value;
        return value;
    }
};

}

#endif // REGISTRABLE_H
