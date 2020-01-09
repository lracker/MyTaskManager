// CMyListCtrl.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CMyListCtrl.h"
#include <tchar.h>
#include "CTools.h"


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}


void CMyListCtrl::AddColumn(DWORD dwCount, ...)
{
	va_list argptr;
	va_start(argptr, dwCount);
	CRect cRect;
	GetClientRect(cRect);
	for (int i = 0; i < dwCount; i++)
	{
		TCHAR* pszColumnName = va_arg(argptr, TCHAR*);
		InsertColumn(i, pszColumnName, 0, cRect.Width()/dwCount);  //插入列
	}
	va_end(argptr);
}

//*****************************************************************************************
// 函数名称: UpdateProList
// 函数说明: 更新列表
// 作    者: lracker
// 时    间: 2019/09/06
// 返 回 值: void
//*****************************************************************************************
void CMyListCtrl::UpdateProList()
{
	if (!GetAllProcess(m_NewProList))  //获取进程列表
	{
		return;
	}
	if (m_OldProList.empty())  //如果是空的话，那说明了是第一次更新列表
	{
		int nIndex = 0;   //列表的每一项
		CString buffer;  //用来转换PID和内存占用
		for (auto& i : m_NewProList)
		{
			InsertItem(nIndex, _T(""));
			buffer.Format(_T("%d"), i.th32ProcessID);
			SetItemText(nIndex, 0, i.szExeFile);
			SetItemText(nIndex, 1, buffer);
			buffer.Format(_T("%.2lfMB"), (double)i.dwMemoryUsage / 1024 / 1024);
			SetItemText(nIndex, 2, buffer);
		/*	buffer.Format(_T("%d"), i.cntThreads);
			SetItemText(nIndex, 3, buffer);*/
			nIndex++;
		}
	}
	else  //如果不是第一次更新，则需要刷新列表，通过比较前后两次进程列表看看有没有变化
	{
		/*先找出退出的进程*/
		int nIndex = 0;
		for(auto&i:m_OldProList)
		{
			if (GetIndex(m_NewProList, i.th32ProcessID) == -1)  //如果找不到的话，那就删掉
			{
				DeleteItem(nIndex);  //删除那一项
				continue; 
			}
			nIndex++;
		}
		/*接着找出新的进程*/
		for (auto& i : m_NewProList)
		{
			if (GetIndex(m_OldProList, i.th32ProcessID) == -1)
			{
				CString buffer;
				InsertItem(nIndex, _T(""));
				SetItemText(nIndex, 0, i.szExeFile);  //插入进程名
				buffer.Format(_T("%d"), i.th32ProcessID);
  				SetItemText(nIndex, 1, buffer);       //插入进程ID
				buffer.Format(_T("%.2lfMB"), (double)i.dwMemoryUsage / 1024 / 1024);  //插入内存占用
				SetItemText(nIndex, 2, buffer);
				nIndex++;
				/*buffer.Format(_T("%d"), i.cntThreads);
				SetItemText(nIndex, 3, buffer);*/
			}
		
		}
	}
	m_OldProList.swap(m_NewProList);  //将新的进程列表和旧的交换一遍
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CMyListCtrl 消息处理程序


