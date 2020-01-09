// CMyTabCtrl.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CMyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


//*****************************************************************************************
// 函数名称: AddTab
// 函数说明: 添加菜单栏
// 作    者: lracker
// 时    间: 2019/09/06
// 参    数: DWORD dwCount
// 参    数: ...
// 返 回 值: void
//*****************************************************************************************
void CMyTabCtrl::AddTab(DWORD dwCount, ...)
{
	va_list argptr;
	va_start(argptr, dwCount);
	for (int i = 0; i < dwCount; i++)
	{
		TCHAR* pszTabName = va_arg(argptr, TCHAR*);
		InsertItem(i, pszTabName);
	}
	va_end(argptr);
	m_dwCount = dwCount;
}

//*****************************************************************************************
// 函数名称: AddDlg
// 函数说明: 添加对话框资源
// 作    者: lracker
// 时    间: 2019/09/06
// 参    数: DWORD dwCount
// 参    数: ...
// 返 回 值: void
//*****************************************************************************************
void CMyTabCtrl::AddDlg(DWORD dwCount, ...)
{
	va_list argptr;
	va_start(argptr, dwCount);
	for (int i = 0; i < dwCount; i++)
	{
		UINT uID = va_arg(argptr, UINT);
		m_Dlg[i] = va_arg(argptr, CDialogEx*);
		m_Dlg[i]->Create(uID, this);
	}
	va_end(argptr);
}

//*****************************************************************************************
// 函数名称: SelectAndShow
// 函数说明: 被选中的显示出来
// 作    者: lracker
// 时    间: 2019/09/06
// 参    数: int nSel
// 返 回 值: void
//*****************************************************************************************
void CMyTabCtrl::SelectAndShow(int nSel)
{
	CRect cRect;
	GetClientRect(cRect);
	cRect.DeflateRect(1, 30, 2, 2);
	SetCurSel(nSel);
	m_Dlg[nSel]->ShowWindow(SW_SHOW);
	m_Dlg[nSel]->MoveWindow(cRect);
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序




void CMyTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	for (int i = 0; i < m_dwCount; i++)
	{
		m_Dlg[i]->ShowWindow(SW_HIDE);  //先将所有的隐藏掉。
	}
	SelectAndShow(GetCurSel());  //将选中的那一项传进去，并且显示出来
	*pResult = 0;
}
