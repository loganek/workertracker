/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef MSWINDOWSSINGLEAPPLOCKER_H
#define MSWINDOWSSINGLEAPPLOCKER_H

#include "singleapplocker.h"

#include <Windows.h>

namespace WT {

class MSWindowsSingleAppLocker : public SingleAppLocker
{
	static RegistrarSingle<MSWindowsSingleAppLocker> registrar;

	HANDLE h_mutex = nullptr;

public:
	virtual ~MSWindowsSingleAppLocker() { unlock(); }
	bool lock() override;
	void unlock() override;
};

}

#endif // MSWINDOWSSINGLEAPPLOCKER_H
