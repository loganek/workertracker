/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
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

	static std::string wchar_to_stdstring(const wchar_t* wstr);

public:
	virtual ~MSWindowsWindowInfoProvider() {}

	Info get_current_window_info() override;
	bool initialize(const std::shared_ptr<WIProviderConfiguration> &) override { return true;  }

	static BOOL CALLBACK enum_chind_windows_callback(HWND hWnd, LPARAM lp);
};

}

#endif
