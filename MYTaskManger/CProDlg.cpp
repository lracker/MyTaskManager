// CProDia.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CProDlg.h"
#include "afxdialogex.h"
#include "CEnumThread.h"
#include "CEnumModule.h"
#include "CEnumHeap.h"
#define UM_CPU (WM_USER+100)


// CProDia 对话框

IMPLEMENT_DYNAMIC(CProDlg, CDialogEx)

CProDlg::CProDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ProDlg, pParent)
{

}

CProDlg::~CProDlg()
{
}

void CProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_MyListCtrl);
}


BEGIN_MESSAGE_MAP(CProDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CProDlg::OnLvnItemchangedList1)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CProDlg::OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CProDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProDlg::OnRclickList1)
	ON_COMMAND(ID_32773, &CProDlg::OnGetThread)
	ON_COMMAND(ID_32774, &CProDlg::OnGetModule)
	ON_COMMAND(ID_32775, &CProDlg::OnEnumHeap)
	ON_MESSAGE(UM_CPU, &CProDlg::OnCpu)
END_MESSAGE_MAP()


// CProDia 消息处理程序


void CProDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

DWORD WINAPI ThreadPro1(LPVOID lpThreadParameter)
{
	while (1)
	{
		int nCPU = GetCpuUsage();
		CProDlg* _this = (CProDlg*)lpThreadParameter;
		SendMessage(_this->m_hWnd, UM_CPU, (WPARAM)nCPU, 0);
		/*Sleep(1500);*/
	}
}

BOOL CProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Menu.LoadMenuW(IDR_MENU2);
	m_MyListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | //选中整行
								  LVS_EX_GRIDLINES);     //网格形式
	m_MyListCtrl.AddColumn(3, L"进程名", L"PID", L"内存占用");  //插入列
	m_MyListCtrl.UpdateProList();  //初始化的时候先显示出来
	m_StatusBar = new CStatusBar();
	m_StatusBar->Create(this);
	UINT uId2[] = { IDS_STRING150, 0x1001 };  //ID数组
	m_StatusBar->SetIndicators(uId2, 2);  //划分窗口
	m_StatusBar->SetPaneInfo(1, 0x1001, 0, 700);
	CString buffer;
	buffer.Format(_T("%d"), GetCpuUsage());
	m_StatusBar->SetPaneText(1, buffer + '%');  //显示内存占用率
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);  
	/*创建一个新的线程*/
	HANDLE hThread1 = CreateThread(NULL, NULL, ThreadPro1, this, NULL, NULL);
	SetTimer(0, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_MyListCtrl.UpdateProList();  //更新进程列表
	CString buffer;
	CDialogEx::OnTimer(nIDEvent);
}


void CProDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString szPid;
	szPid = m_MyListCtrl.GetItemText(pNMItemActivate->iItem, 1);  //获取选中项的PID
	_stscanf_s(szPid, _T("%d"), &m_dwPid);  //转为DWORD类型的PID，传给成员变量m_dwPid
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);  //设置按钮可用
	*pResult = 0;
}


void CProDlg::OnBnClickedButton1()
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwPid);
	TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  //设置按钮不可用
}


void CProDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT point = { 0 };
	GetCursorPos(&point);  //获取鼠标坐标
	CMenu* SubMenu = m_Menu.GetSubMenu(0);
	SubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}


//*****************************************************************************************
// 函数名称: OnGetThread
// 函数说明: 遍历线程
// 作    者: lracker
// 时    间: 2019/09/06
// 返 回 值: void
//*****************************************************************************************
void CProDlg::OnGetThread()
{
	CEnumThread EnumThreadobj;
	EnumThreadobj.m_dwPid = m_dwPid;
	EnumThreadobj.DoModal();
}


void CProDlg::OnGetModule()
{
	CEnumModule EnumModule;
	EnumModule.m_dwPid = m_dwPid;
	EnumModule.DoModal();
}


void CProDlg::OnEnumHeap()
{
	CEnumHeap EnumHeap;
	EnumHeap.m_dwPid = m_dwPid;
	EnumHeap.DoModal();
}



afx_msg LRESULT CProDlg::OnCpu(WPARAM wParam, LPARAM lParam)
{
	CString buffer;
	buffer.Format(_T("%d"), (int)wParam);
	m_StatusBar->SetPaneText(1, buffer + '%');
	return 0;
}
