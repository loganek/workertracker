#include "mswindowsservice.h"
#include "threadpool.h"

#include "wtcommon/logger.h"

namespace WT {

MSWindowsService::RegistrarSingle<MSWindowsService> MSWindowsService::registrar((PTSTR)WT_PROJECT_NAME);

MSWindowsService::MSWindowsService(PTSTR service_name)
	: CServiceBase(service_name, TRUE, TRUE, FALSE)
{
}


MSWindowsService::~MSWindowsService(void)
{
	if (stopped_event)
	{
		CloseHandle(stopped_event);
		stopped_event = NULL;
	}
}

void MSWindowsService::OnStart(DWORD dwArgc, LPTSTR *lpszArgv)
{
	WriteEventLogEntry("CppWindowsService in OnStart",
		EVENTLOG_INFORMATION_TYPE);

	CThreadPool::QueueUserWorkItem(&MSWindowsService::worker_thread, this);
}

void MSWindowsService::worker_thread()
{
	ret_value = callback();

	SetEvent(stopped_event);
}

void MSWindowsService::OnStop()
{
	WriteEventLogEntry("CppWindowsService in OnStop",
		EVENTLOG_INFORMATION_TYPE);

	stopping = true;
	if (WaitForSingleObject(stopped_event, INFINITE) != WAIT_OBJECT_0)
	{
		throw GetLastError();
	}
}

int MSWindowsService::run_in_background(std::function<int()> callback)
{
	this->callback = callback;
	
	stopped_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (stopped_event == NULL)
	{
		throw GetLastError();
	}

	if (!CServiceBase::Run(*this))
	{
		WT_LOG_ERR << "Cannot run application in background: " << GetLastError();
		ret_value = -1;
	}

	return ret_value;
}

}