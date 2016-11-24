/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "mswindowswindowinfoprovider.h"

#include <tlhelp32.h>

namespace WT {

MSWindowsWindowInfoProvider::RegistrarSingle<MSWindowsWindowInfoProvider> MSWindowsWindowInfoProvider::registrar;

std::string MSWindowsWindowInfoProvider::read_process_name(DWORD pid)
{
	std::string process_name = "(unknown)";
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pid);
	if (hSnapshot)
	{
		PROCESSENTRY32W peInfo;
		peInfo.dwSize = sizeof(peInfo);
		BOOL nextProcess = Process32FirstW(hSnapshot, &peInfo);
		bool found = false;
		while (nextProcess)
		{
			if (peInfo.th32ProcessID == pid)
			{
				found = true;
				break;
			}
			nextProcess = Process32NextW(hSnapshot, &peInfo);
		}
		if (found)
		{
			std::wstring ws(peInfo.szExeFile);
			process_name = std::string(ws.begin(), ws.end());
		}
		CloseHandle(hSnapshot);
	}

	return process_name;
}

std::string MSWindowsWindowInfoProvider::read_window_title(HWND winHandle)
{
	char buf[2048];
	int ret = GetWindowTextA(winHandle, buf, 2048);
	// TODO duplicate "(unknown) (see X provider)"
	return !ret ? "(unknown)" : buf;
}

WindowInfoProvider::Info MSWindowsWindowInfoProvider::get_current_window_info()
{
	Info info;

	HWND winHandle = GetForegroundWindow();
	info.window_title = read_window_title(winHandle);

	DWORD pid;
	GetWindowThreadProcessId(winHandle, &pid);
	info.app_name = read_process_name(pid);

	return info;
}

}