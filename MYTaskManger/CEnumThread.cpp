// CEnumThread.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CEnumThread.h"
#include "afxdialogex.h"



// CEnumThread 对话框

IMPLEMENT_DYNAMIC(CEnumThread, CDialogEx)

CEnumThread::CEnumThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnumThread, pParent)
{

}

CEnumThread::~CEnumThread()
{
}

void CEnumThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}



//*****************************************************************************************
// 函数名称: GetThread
// 函数说明: 获取线程
// 作    者: lracker
// 时    间: 2019/09/06
// 返 回 值: void
//*****************************************************************************************
bool CEnumThread::GetThread()
{
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return false;
	THREADENTRY32 te = { sizeof(THREADENTRY32) };
	if (!Thread32First(hThreadSnap, &te))
		return false;
	int nIndex = 0;
	CString OwnerPid;
	OwnerPid.Format(_T("%d"), m_dwPid);
	do 
	{
		if (te.th32OwnerProcessID == m_dwPid)
		{
			CString buffer;
			m_ThreadList.push_back(te);
			m_List.InsertItem(nIndex, _T(""));
			buffer.Format(_T("%d"), nIndex+1);
			m_List.SetItemText(nIndex, 0, buffer);
			m_List.SetItemText(nIndex,1,OwnerPid);
			buffer.Format(_T("%d"), te.th32ThreadID);  //线程标识号
			m_List.SetItemText(nIndex, 2, buffer);
			buffer.Format(_T("%d"), te.tpBasePri);
			m_List.SetItemText(nIndex, 3, buffer);
			nIndex++;
		}
	} while (Thread32Next(hThreadSnap,&te));
}

BEGIN_MESSAGE_MAP(CEnumThread, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEnumThread::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CEnumThread::OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEnumThread::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEnumThread::OnBnClickedButton3)
END_MESSAGE_MAP()


// CEnumThread 消息处理程序

BOOL CEnumThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect cRect;
	GetClientRect(cRect);
	m_List.InsertColumn(0, L"序号", 0, cRect.Width() / 4);
	m_List.InsertColumn(1, L"线程拥有者PID", 0, cRect.Width()/4);
	m_List.InsertColumn(2, L"线程TID", 0, cRect.Width() / 4);
	m_List.InsertColumn(3, L"线程优先级", 0, cRect.Width() / 4);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetThread();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CEnumThread::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_dwSelect =pNMItemActivate->iItem;  //获取选中第几项
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);  //设置按钮可用
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);  //设置按钮可用
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);  //设置按钮可用
	*pResult = 0;
}

//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 暂停线程
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: void
//*****************************************************************************************
void CEnumThread::OnBnClickedButton1()
{
	CString szTid;
	DWORD dwTid;
	szTid = m_List.GetItemText(m_dwSelect, 2);  //获取选中项的PID
	_stscanf_s(szTid, _T("%d"), &dwTid);  //转为DWORD类型的TID，传给成员变量m_dwTid
	HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwTid);  //获取指定线程的句柄
	if (hThread == NULL)
		return;
	SuspendThread(hThread);  //否则暂停该线程
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  //设置按钮不可用
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  //设置按钮不可用
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);  //设置按钮不可用
}

//*****************************************************************************************
// 函数名称: OnBnClickedButton2
// 函数说明: 恢复线程
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: void
//*****************************************************************************************
void CEnumThread::OnBnClickedButton2()
{
	CString szTid;
	DWORD dwTid;
	szTid = m_List.GetItemText(m_dwSelect, 2);  //获取选中项的PID
	_stscanf_s(szTid, _T("%d"), &dwTid);  //转为DWORD类型的TID，传给成员变量m_dwTid
	HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwTid);  //获取指定线程的句柄
	if (hThread == NULL)
		return;
	ResumeThread(hThread);  //否则恢复该线程
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  //设置按钮不可用
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  //设置按钮不可用
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);  //设置按钮不可用
}


//*****************************************************************************************
// 函数名称: OnBnClickedButton3
// 函数说明: 结束线程
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: void
//*****************************************************************************************
void CEnumThread::OnBnClickedButton3()
{
	CString szTid;
	DWORD dwTid;
	szTid = m_List.GetItemText(m_dwSelect, 2);  //获取选中项的PID
	_stscanf_s(szTid, _T("%d"), &dwTid);  //转为DWORD类型的TID，传给成员变量m_dwTid
	HANDLE hThread = OpenThread(THREAD_TERMINATE, FALSE, dwTid);  //获取指定线程的句柄
	if (hThread == NULL)
		return;
	TerminateThread(hThread, 0);  //否则恢复该线程
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  //设置按钮不可用
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  //设置按钮不可用
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);  //设置按钮不可用
	m_List.DeleteItem(m_dwSelect);
}
