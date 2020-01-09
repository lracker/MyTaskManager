// CUserDlg.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CFileDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>
#include "CVSRUBBISH.h"



// CUserDlg 对话框

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FileDlg, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)

	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CFileDlg::OnClickTree1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CFileDlg::OnRclickTree1)
	ON_COMMAND(ID_32778, &CFileDlg::OnSCANVS)
END_MESSAGE_MAP()


// CUserDlg 消息处理程序


//*****************************************************************************************
// 函数名称: OnInitDialog
// 函数说明: 初始化左边和右边
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: BOOL
//*****************************************************************************************
BOOL CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Tree.InitTree();
	CRect cRect;
	m_List.GetClientRect(cRect);
	m_List.InsertColumn(0, L"文件名", 0, cRect.Width() / 4);
	m_List.InsertColumn(1, L"文件大小", 0, cRect.Width() / 4);
	m_List.InsertColumn(2, L"创建时间", 0, cRect.Width() / 4);
	m_List.InsertColumn(3, L"最后修改时间", 0, cRect.Width() / 4);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Menu.LoadMenuW(IDR_MENU3);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//*****************************************************************************************
// 函数名称: OnClickTree1
// 函数说明: 点击目录树，展开，目录在左边显示，右边显示文件
// 作    者: lracker
// 时    间: 2019/09/07
// 参    数: NMHDR * pNMHDR
// 参    数: LRESULT * pResult
// 返 回 值: void
//*****************************************************************************************
void CFileDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pos = {};
	GetCursorPos(&pos);
	ScreenToClient(&pos);
	HTREEITEM hItem = m_Tree.HitTest(pos);
	CString Path = (wchar_t*)m_Tree.GetItemData(hItem);
	HTREEITEM hChild = m_Tree.GetNextItem(hItem, TVGN_CHILD);
	if (hChild) // 有点问题
		return;
	//开始遍历文件
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = FindFirstFile(Path + L"\\*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	m_List.DeleteAllItems();  //删掉所有的节点
	DWORD dwFileCount = 0;
	do {
		if (!lstrcmp(FindFileData.cFileName, L".") || !lstrcmp(FindFileData.cFileName, L".."))
			continue;
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  //如果是目录的话那就添加到左边那一栏
		{
			HTREEITEM hItem2 = m_Tree.InsertItem(FindFileData.cFileName, hItem);
			WCHAR* szFullPath = new WCHAR[MAX_PATH];
			StringCbPrintf(szFullPath, MAX_PATH, L"%s\\%s", Path, FindFileData.cFileName);  //拼接起来
			m_Tree.SetItemData(hItem2, (DWORD_PTR)szFullPath);
		}
		else //如果不是目录的话，那么将该目录名发送到
		{
			m_List.InsertItem(dwFileCount, FindFileData.cFileName);
			ULONG64 size = (FindFileData.nFileSizeHigh << 32) | FindFileData.nFileSizeLow;
			CString buffer;
			buffer.Format(L"%lldkb", size / 1024);
			m_List.SetItemText(dwFileCount, 1, buffer);  //设置大小
			FILETIME ftLocal;
			SYSTEMTIME sysTime = { 0 };
			FileTimeToLocalFileTime(&FindFileData.ftCreationTime, &ftLocal);
			FileTimeToSystemTime(&ftLocal, &sysTime);
			buffer.Format(L"%04d/%02d/%02d %02d:%02d:%02d",
				sysTime.wYear,
				sysTime.wMonth,
				sysTime.wDay,
				sysTime.wHour,
				sysTime.wMinute,
				sysTime.wSecond);
			m_List.SetItemText(dwFileCount, 2, buffer);
			FileTimeToLocalFileTime(&FindFileData.ftLastWriteTime, &ftLocal);
			FileTimeToSystemTime(&ftLocal, &sysTime);
			buffer.Format(L"%04d/%02d/%02d %02d:%02d:%02d",
				sysTime.wYear,
				sysTime.wMonth,
				sysTime.wDay,
				sysTime.wHour,
				sysTime.wMinute,
				sysTime.wSecond);
			m_List.SetItemText(dwFileCount, 3, buffer);

		}

	} while (FindNextFile(hFind, &FindFileData));
	*pResult = 0;
}


//*****************************************************************************************
// 函数名称: OnRclickTree1
// 函数说明: 右键弹出菜单
// 作    者: lracker
// 时    间: 2019/09/07
// 参    数: NMHDR * pNMHDR
// 参    数: LRESULT * pResult
// 返 回 值: void
//*****************************************************************************************
void CFileDlg::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMenu* hSubMenu = m_Menu.GetSubMenu(0);
	POINT point;
	GetCursorPos(&point);
	m_Tree.ScreenToClient(&point);
	HTREEITEM hItem = m_Tree.HitTest(point);
	if (!hItem)
		m_CurPath.Empty();
	else
		m_CurPath = (wchar_t*)m_Tree.GetItemData(hItem);
	GetCursorPos(&point);
	hSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}


//*****************************************************************************************
// 函数名称: OnSCANVS
// 函数说明: 根据选定的文件夹来扫除VS的垃圾。
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: void
//*****************************************************************************************
void CFileDlg::OnSCANVS()
{
	CVSRUBBISH Rubbish;
	Rubbish.m_CurPath = m_CurPath;  //把路径给新的窗口
	Rubbish.DoModal();
}

