#include "pch.h"
#include "CTools.h"
#include <psapi.h>

//*****************************************************************************************
// 函数名称: GetAllProcess
// 函数说明: 获取所有进程并且获取内存占用等信息
// 作    者: lracker
// 时    间: 2019/09/06
// 参    数: _Out_ vector<PROCESSINFO> & proList
// 返 回 值: bool
//*****************************************************************************************
bool GetAllProcess(_Out_ vector<PROCESSINFO>& ProList)
{
	ProList.clear();
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
		return false;  //如果获取快照失败的话返回false
	/*从第一个开始遍历*/
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };  //初始化一个结构体
	if (!Process32First(hSnapShot, &pe))   //如果获取第一个进程信息失败，则返回false
		return false;
	do 
	{
		PROCESSINFO pInfo = *(PROCESSINFO*) &pe;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
		if (!hProcess)
			pInfo.dwMemoryUsage = 0;  //如果获取句柄失败的话，那么就设置内存占用为0
		else
		{
			PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
			GetProcessMemoryInfo(hProcess, &pmc, sizeof(PROCESS_MEMORY_COUNTERS));
			pInfo.dwMemoryUsage = pmc.WorkingSetSize;  //获取内存占用
		}
		ProList.push_back(pInfo);
	} while (Process32Next(hSnapShot,&pe));
	return true;
}

//*****************************************************************************************
// 函数名称: GetIndex
// 函数说明: 获取在列表中的下标
// 作    者: lracker
// 时    间: 2019/09/06
// 参    数: _In_ vector<PROCESSINFO> & ProList
// 参    数: DWORD dwPid
// 返 回 值: int
//			 -1为找不到
//*****************************************************************************************
int GetIndex(_In_ vector<PROCESSINFO>& ProList, DWORD dwPid)
{	
	for (int i = 0; i < ProList.size(); i++)
	{
		if (ProList[i].th32ProcessID == dwPid)  //如果找到该程序，返回下标
		{
			return i;
		}
	}
	return -1;
}

//*****************************************************************************************
// 函数名称: GetVsRubbish
// 函数说明: 获取VS垃圾
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: bool
//*****************************************************************************************
bool GetVsRubbish(CString dir, std::vector<CString> &paths)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile = FindFirstFile(dir + "\\*", &FindFileData);
	if (hListFile == INVALID_HANDLE_VALUE)
		return false;
	do 
	{
		if (!lstrcmp(FindFileData.cFileName, L".") || !lstrcmp(FindFileData.cFileName, L".."))
			continue;
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  //如果是个目录的话
			GetVsRubbish(dir + L"\\" + FindFileData.cFileName, paths);
		else
		{
			// 判断是否是vs垃圾文件的后缀
			const TCHAR* extName[] = { L".ilk",L".pdb",L".obj",L".log",L".pch",L".tlog",L".lastbuildstate",L".sdf",L".idb",L".ipch",L".res",L".o",L".lst",L".knl",L".img",L".bin",L".db" };
			for (auto& i : extName)
			{
				TCHAR* pExtName = FindFileData.cFileName + wcslen(FindFileData.cFileName);
				while (pExtName != FindFileData.cFileName & *pExtName != '.')
					pExtName--;
				if (!_wcsicmp(pExtName, i))//如果是垃圾后缀的话，那就压入
					paths.push_back(dir + L"\\" + FindFileData.cFileName);  
			}
		}
	} while (FindNextFile(hListFile,&FindFileData));
}

double FileTimeToDouble(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime * 4.294967296e9) + double(fileTime.dwLowDateTime);
}
int GetCpuUsage()
{
	_FILETIME idleTime, kernelTime, userTime;  //空闲时间，内核时间，用户时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);  //获取时间
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);  //创建事件
	WaitForSingleObject(hEvent, 1000);  //等待1000毫秒，使用内核对象等待会更精确
	_FILETIME newIdleTime, newKernelTime, newUserTime;  //获取新的时间
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//将各个时间转换
	double dwOldIdleTime = FileTimeToDouble(idleTime);
	double dwNewIdleTime = FileTimeToDouble(newIdleTime);
	double dwOldKernelTime = FileTimeToDouble(kernelTime);
	double dwNewKernelTime = FileTimeToDouble(newKernelTime);
	double dwOldUserTime = FileTimeToDouble(userTime);
	double dwNewUserTime = FileTimeToDouble(newUserTime);
	return int(100.0 - (dwNewIdleTime - dwOldIdleTime) / (dwNewKernelTime - dwOldKernelTime + dwNewUserTime - dwOldUserTime) * 100.0);
}



//************************************************************************
// 函数名: EnableDebugPrivilege
/// 功  能: 将程序的权限提升为SeDebug权限
// 全  名: EnableDebugPrivilege
// 权  限: public
// 返回值: bool
// 形  参: BOOL bIsOpen
//************************************************************************
bool EnableDebugPrivilege()
{
	HANDLE hToken = NULL;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	TOKEN_ELEVATION_TYPE ElevationType;
	DWORD dwSize = 0;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		MessageBoxA(NULL, "打开失败", 0, 0);
		return   FALSE;
	}


	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		MessageBoxA(NULL, "获取失败", 0, 0);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // 选择还是关闭

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		MessageBoxA(NULL, "获取权限失败", 0, 0);
		return false;
	}
	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
	{
		// 如果令牌是以受限的权限运行 (TokenElevationTypeLimited) ,
		if (ElevationType == TokenElevationTypeLimited)
		{
			//MessageBox( NULL , L"您必须以管理员身份运行,否则无法获取获取系统的内存使用率" , 0 , 0 );
			return	FALSE;
		}
	}
	return true;
}

bool getProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
{
	// 1. 先给当前系统所有的进程拍一个快照.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 2. 从快照中提取出进程信息
	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
	if (!Module32First(hSnap, &mInfo)) {
		return FALSE;
	}
	do
	{
		moduleList->push_back(mInfo);
	} while (Module32Next(hSnap, &mInfo));
	return true;
}

