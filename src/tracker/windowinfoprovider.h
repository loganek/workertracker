#ifndef WINDOWINFOPROVIDER_H
#define WINDOWINFOPROVIDER_H

#include "wtcommon/registrable.h"

#include <string>

namespace WT {

class WindowInfoProvider : public RegistrableSingle<WindowInfoProvider>
{
public:
    struct Info
    {
        std::string window_title;
        std::string app_name;
    };

    virtual ~WindowInfoProvider() {}

    virtual Info get_current_window_info() const = 0;
};

}

#endif // WINDOWINFOPROVIDER_H
