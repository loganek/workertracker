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

#include <algorithm>

namespace WT {

struct PIDHierarchyInfo
{
	DWORD ownerpid;
	DWORD childpid;
};

MSWindowsWindowInfoProvider::RegistrarSingle<MSWindowsWindowInfoProvider> MSWindowsWindowInfoProvider::registrar;

BOOL CALLBACK MSWindowsWindowInfoProvider::enum_chind_windows_callback(HWND hWnd, LPARAM lp)
{
	PIDHierarchyInfo* info = (PIDHierarchyInfo*)lp;
	DWORD pid = 0;
	GetWindowThreadProcessId(hWnd, &pid);
	if (pid != info->ownerpid) info->childpid = pid;
	return TRUE;
}

std::string MSWindowsWindowInfoProvider::read_process_name(DWORD pid)
{
	std::string process_name;
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
			process_name = wchar_to_stdstring(peInfo.szExeFile);
		}
		CloseHandle(hSnapshot);
	}

	return process_name;
}

std::string MSWindowsWindowInfoProvider::wchar_to_stdstring(const wchar_t* wstr)
{
	std::mbstate_t state = std::mbstate_t();
	std::wstring ws(wstr);
	ws.erase(std::remove_if(ws.begin(), ws.end(), iswcntrl));
	wstr = ws.c_str();
	int len = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
	if (len)
	{
		std::vector<char> mbstr(len);
		std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);
		return &mbstr[0];
	}
	else
	{
		return "";
	}
}

std::string MSWindowsWindowInfoProvider::read_window_title(HWND winHandle)
{
	wchar_t buf[2048];
	int ret = GetWindowTextW(winHandle, buf, 2048);
	std::string value = wchar_to_stdstring(buf);
	return !ret ? "" : value;
}

DWORD get_window_pid(HWND winHandle)
{
	DWORD pid;
	
	GetWindowThreadProcessId(winHandle, &pid);
	
	char winClassName[MAX_CLASS_NAME];
	int ret = GetClassName(winHandle, winClassName, 1024);
	
	if (!ret || strcmp(winClassName, "ApplicationFrameWindow") != 0)
	{
		return pid;
	}

	PIDHierarchyInfo info = { 0 };
	info.ownerpid = pid;
	info.childpid = info.ownerpid;
	EnumChildWindows(winHandle, &MSWindowsWindowInfoProvider::enum_chind_windows_callback, (LPARAM)&info);
	return info.childpid;
}

WindowInfoProvider::Info MSWindowsWindowInfoProvider::get_current_window_info()
{
	HWND winHandle = GetForegroundWindow();
	auto window_title = read_window_title(winHandle);

	auto app_name = read_process_name(get_window_pid(winHandle));

	return Info(app_name, window_title);
}

}