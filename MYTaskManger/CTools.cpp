#include "pch.h"
#include "CTools.h"
#include <psapi.h>

//*****************************************************************************************
// ��������: GetAllProcess
// ����˵��: ��ȡ���н��̲��һ�ȡ�ڴ�ռ�õ���Ϣ
// ��    ��: lracker
// ʱ    ��: 2019/09/06
// ��    ��: _Out_ vector<PROCESSINFO> & proList
// �� �� ֵ: bool
//*****************************************************************************************
bool GetAllProcess(_Out_ vector<PROCESSINFO>& ProList)
{
	ProList.clear();
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
		return false;  //�����ȡ����ʧ�ܵĻ�����false
	/*�ӵ�һ����ʼ����*/
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };  //��ʼ��һ���ṹ��
	if (!Process32First(hSnapShot, &pe))   //�����ȡ��һ��������Ϣʧ�ܣ��򷵻�false
		return false;
	do 
	{
		PROCESSINFO pInfo = *(PROCESSINFO*) &pe;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
		if (!hProcess)
			pInfo.dwMemoryUsage = 0;  //�����ȡ���ʧ�ܵĻ�����ô�������ڴ�ռ��Ϊ0
		else
		{
			PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
			GetProcessMemoryInfo(hProcess, &pmc, sizeof(PROCESS_MEMORY_COUNTERS));
			pInfo.dwMemoryUsage = pmc.WorkingSetSize;  //��ȡ�ڴ�ռ��
		}
		ProList.push_back(pInfo);
	} while (Process32Next(hSnapShot,&pe));
	return true;
}

//*****************************************************************************************
// ��������: GetIndex
// ����˵��: ��ȡ���б��е��±�
// ��    ��: lracker
// ʱ    ��: 2019/09/06
// ��    ��: _In_ vector<PROCESSINFO> & ProList
// ��    ��: DWORD dwPid
// �� �� ֵ: int
//			 -1Ϊ�Ҳ���
//*****************************************************************************************
int GetIndex(_In_ vector<PROCESSINFO>& ProList, DWORD dwPid)
{	
	for (int i = 0; i < ProList.size(); i++)
	{
		if (ProList[i].th32ProcessID == dwPid)  //����ҵ��ó��򣬷����±�
		{
			return i;
		}
	}
	return -1;
}

//*****************************************************************************************
// ��������: GetVsRubbish
// ����˵��: ��ȡVS����
// ��    ��: lracker
// ʱ    ��: 2019/09/07
// �� �� ֵ: bool
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
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  //����Ǹ�Ŀ¼�Ļ�
			GetVsRubbish(dir + L"\\" + FindFileData.cFileName, paths);
		else
		{
			// �ж��Ƿ���vs�����ļ��ĺ�׺
			const TCHAR* extName[] = { L".ilk",L".pdb",L".obj",L".log",L".pch",L".tlog",L".lastbuildstate",L".sdf",L".idb",L".ipch",L".res",L".o",L".lst",L".knl",L".img",L".bin",L".db" };
			for (auto& i : extName)
			{
				TCHAR* pExtName = FindFileData.cFileName + wcslen(FindFileData.cFileName);
				while (pExtName != FindFileData.cFileName & *pExtName != '.')
					pExtName--;
				if (!_wcsicmp(pExtName, i))//�����������׺�Ļ����Ǿ�ѹ��
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
	_FILETIME idleTime, kernelTime, userTime;  //����ʱ�䣬�ں�ʱ�䣬�û�ʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);  //��ȡʱ��
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);  //�����¼�
	WaitForSingleObject(hEvent, 1000);  //�ȴ�1000���룬ʹ���ں˶���ȴ������ȷ
	_FILETIME newIdleTime, newKernelTime, newUserTime;  //��ȡ�µ�ʱ��
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//������ʱ��ת��
	double dwOldIdleTime = FileTimeToDouble(idleTime);
	double dwNewIdleTime = FileTimeToDouble(newIdleTime);
	double dwOldKernelTime = FileTimeToDouble(kernelTime);
	double dwNewKernelTime = FileTimeToDouble(newKernelTime);
	double dwOldUserTime = FileTimeToDouble(userTime);
	double dwNewUserTime = FileTimeToDouble(newUserTime);
	return int(100.0 - (dwNewIdleTime - dwOldIdleTime) / (dwNewKernelTime - dwOldKernelTime + dwNewUserTime - dwOldUserTime) * 100.0);
}



//************************************************************************
// ������: EnableDebugPrivilege
/// ��  ��: �������Ȩ������ΪSeDebugȨ��
// ȫ  ��: EnableDebugPrivilege
// Ȩ  ��: public
// ����ֵ: bool
// ��  ��: BOOL bIsOpen
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
		MessageBoxA(NULL, "��ʧ��", 0, 0);
		return   FALSE;
	}


	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		MessageBoxA(NULL, "��ȡʧ��", 0, 0);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // ѡ���ǹر�

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		MessageBoxA(NULL, "��ȡȨ��ʧ��", 0, 0);
		return false;
	}
	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
	{
		// ��������������޵�Ȩ������ (TokenElevationTypeLimited) ,
		if (ElevationType == TokenElevationTypeLimited)
		{
			//MessageBox( NULL , L"�������Թ���Ա�������,�����޷���ȡ��ȡϵͳ���ڴ�ʹ����" , 0 , 0 );
			return	FALSE;
		}
	}
	return true;
}

bool getProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
{
	// 1. �ȸ���ǰϵͳ���еĽ�����һ������.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 2. �ӿ�������ȡ��������Ϣ
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

