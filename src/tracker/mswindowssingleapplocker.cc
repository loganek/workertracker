/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "mswindowssingleapplocker.h"

#include "wtcommon/logger.h"

namespace WT {

MSWindowsSingleAppLocker::RegistrarSingle<MSWindowsSingleAppLocker> MSWindowsSingleAppLocker::registrar;

bool MSWindowsSingleAppLocker::lock()
{
	h_mutex = OpenMutex(MUTEX_ALL_ACCESS, 0, WT_PROJECT_NAME);

	if (h_mutex)
	{
		WT_LOG_ERR << "Mutex already created (i.e. application is already running), exiting.";
		h_mutex = nullptr;
		return false;
	}

	h_mutex = CreateMutex(nullptr, 0, WT_PROJECT_NAME);

	return true;
}

void MSWindowsSingleAppLocker::unlock()
{
	if (h_mutex)
	{
		if (!ReleaseMutex(h_mutex))
		{
			WT_LOG_ERR << "Error on releasing mutex: " << GetLastError();
		}
		else
		{
			h_mutex = nullptr;
		}
	}
}

}
