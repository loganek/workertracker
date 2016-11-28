/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
* ----------------------------------------------------------------------------
*/
#ifndef MSWINDOWSSERVICE_H
#define MSWINDOWSSERVICE_H

#include "mswindowsservicebase.h"
#include "backgroundrunner.h"

namespace WT {

class MSWindowsService : public CServiceBase, public BackgroundRunner
{
public:
	MSWindowsService(PTSTR service_name);
	virtual ~MSWindowsService();

	int run_in_background(std::function<int()> callback) override;
	int kill_process() override { return 1;  }
	void register_kill_method(void(*handler)(int)) override {   }

protected:

	virtual void OnStart(DWORD dwArgc, PTSTR *pszArgv);
	virtual void OnStop();

	void worker_thread(void);

private:
	static RegistrarSingle<MSWindowsService> registrar;
	
	std::function<int()> callback;
	int ret_value;

	bool stopping = false;
	HANDLE stopped_event;
};

}

#endif // MSWINDOWSSERVICE_H
