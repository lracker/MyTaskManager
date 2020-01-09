#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>
using std::vector;

struct PROCESSINFO : public PROCESSENTRY32
{
	DWORD dwMemoryUsage;  //内存使用率
};

bool GetAllProcess(_Out_ vector<PROCESSINFO>& ProList);

int GetIndex(_In_ vector<PROCESSINFO>& ProList, DWORD dwPid);

bool GetVsRubbish(CString dir, std::vector<CString> &paths);

int GetCpuUsage();

bool EnableDebugPrivilege();

bool getProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);