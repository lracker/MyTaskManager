// CEnumWindow.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CEnumWindow.h"
#include "afxdialogex.h"


// CEnumWindow 对话框

IMPLEMENT_DYNAMIC(CEnumWindow, CDialogEx)

CEnumWindow::CEnumWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnumWindow, pParent)
{

}

CEnumWindow::~CEnumWindow()
{
}

void CEnumWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CEnumWindow, CDialogEx)
END_MESSAGE_MAP()


// CEnumWindow 消息处理程序


BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lparam)
{
	WCHAR buff[200];
	GetWindowText(hWnd, buff, 200);  //得到窗口名
	if (IsWindowVisible(hWnd) == TRUE && wcslen(buff) != 0)
	{
		CEnumWindow* p = (CEnumWindow*)lparam;
		p->m_List.InsertItem(0, buff);
	}
	return true;
}

BOOL CEnumWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect cRect;
	GetClientRect(cRect);
	m_List.InsertColumn(0, L"窗口名",0,cRect.Width());
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	EnumWindows(EnumWinProc, (LPARAM)this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
