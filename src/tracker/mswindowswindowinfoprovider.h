#ifndef MSWINDOWSWINDOWINFOPROVIDER_H
#define MSWINDOWSWINDOWINFOPROVIDER_H

#include "windowinfoprovider.h"

#include <Windows.h>

namespace WT {

class MSWindowsWindowInfoProvider : public WindowInfoProvider
{
	std::string read_process_name(DWORD pid);
	std::string read_window_title(HWND winHandle);

	static RegistrarSingle<MSWindowsWindowInfoProvider> registrar;

public:

	virtual ~MSWindowsWindowInfoProvider() {}

	Info get_current_window_info() override;
};

}

#endif
