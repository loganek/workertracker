/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
* ----------------------------------------------------------------------------
*/
#include "mswindowsbgrunner.h"

#include "wtcommon/fileloggermethod.h"
#include "wtcommon/configuration.h"

#include <boost/filesystem/path.hpp>

#include <Windows.h>
#include <TlHelp32.h>

namespace WT {

MSWindowsBGRunner::RegistrarSingle<MSWindowsBGRunner> MSWindowsBGRunner::registrar(WT_PROJECT_NAME);

MSWindowsBGRunner::MSWindowsBGRunner(const std::string &log_file_name)
	: log_file_name(log_file_name)
{
}

int MSWindowsBGRunner::move_to_background()
{
	WT_LOG_D << "Moving application to background...";

	auto log_path = boost::filesystem::path(WT::Configuration::get_default_config_path()).parent_path();
	log_path /= log_file_name + ".log";
	WT_LOG_I << "Log file: " << log_path;

	if (FreeConsole())
	{
		FileLoggerMethod::init_log(log_path.string());
		MethodOutput::set_method(FileLoggerMethod::output);

		WT_LOG_D << "Process has been detached from the console";
		return 1;
	}
	else
	{
		WT_LOG_ERR << "Can't detach process from the console: " << GetLastError();
		return -1;
	}
}

std::string MSWindowsBGRunner::get_current_process_name()
{
	char filename[MAX_PATH];
	GetModuleFileNameA(NULL, filename, MAX_PATH);
	return boost::filesystem::path(filename).filename().string();
}

int MSWindowsBGRunner::kill_process()
{
	std::string filename = get_current_process_name();
	const char *fname = filename.c_str();
	DWORD cur_proc_id = GetCurrentProcessId();
	int ret = -1;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, fname) == 0 && cur_proc_id != pEntry.th32ProcessID)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				ret = TerminateProcess(hProcess, 9) ? 0 : -1;
				CloseHandle(hProcess);
				break;
			}
		}

		hRes = Process32Next(hSnapShot, &pEntry);
	}

	CloseHandle(hSnapShot);

	return ret;
}

void MSWindowsBGRunner::register_kill_method(void(*handler)(int))
{
	// TODO how to handle process termination? (TerminateProcess can't be handled,
	// but we might have another option to kill the app).
}

}