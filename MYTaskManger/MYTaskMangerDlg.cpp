
// MYTaskMangerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MYTaskManger.h"
#include "MYTaskMangerDlg.h"
#include "afxdialogex.h"
#include "CProDlg.h"
#include "CFileDlg.h"
#include "CEnumWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMYTaskMangerDlg 对话框



CMYTaskMangerDlg::CMYTaskMangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYTASKMANGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMYTaskMangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTabCtrl);
}

BEGIN_MESSAGE_MAP(CMYTaskMangerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CMYTaskMangerDlg::OnEnumWindow)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
END_MESSAGE_MAP()


// CMYTaskMangerDlg 消息处理程序

BOOL CMYTaskMangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	m_Menu.LoadMenuW(IDR_MENU1);
	SetMenu(&m_Menu);
	m_MyTabCtrl.AddTab(2, L"进程", L"文件管理器");  //插入TAB的菜单栏
	m_MyTabCtrl.m_Dlg[0] = new CProDlg();//建立资源框
	m_MyTabCtrl.m_Dlg[1] = new CFileDlg();    //建立资源框
	m_MyTabCtrl.AddDlg(2, IDD_ProDlg, m_MyTabCtrl.m_Dlg[0],
						  IDD_FileDlg, m_MyTabCtrl.m_Dlg[1]);  //插入两个资源框
	m_MyTabCtrl.SelectAndShow(0);  //默认显示第一个资源框
	EnableDebugPrivilege();  //提权
	/*注册热键*/
	RegisterHotKey(m_hWnd, 1001, MOD_CONTROL | MOD_SHIFT, 'y');
	RegisterHotKey(m_hWnd, 1002, MOD_CONTROL | MOD_SHIFT, 'Y');
	RegisterHotKey(m_hWnd, 1003, MOD_CONTROL | MOD_SHIFT, 't');
	RegisterHotKey(m_hWnd, 1004, MOD_CONTROL | MOD_SHIFT, 'T');
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMYTaskMangerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMYTaskMangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//*****************************************************************************************
// 函数名称: OnEnumWindow
// 函数说明: 遍历窗口
// 作    者: lracker
// 时    间: 2019/09/06
// 返 回 值: void
//*****************************************************************************************
void CMYTaskMangerDlg::OnEnumWindow()  
{
	CEnumWindow EnumWindow;
	EnumWindow.DoModal();  //创建一个模态对话框
}


void CMYTaskMangerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	UnregisterHotKey(m_hWnd, 1001);
	UnregisterHotKey(m_hWnd, 1002);
	UnregisterHotKey(m_hWnd, 1003);
	UnregisterHotKey(m_hWnd, 1004);
	// TODO: 在此处添加消息处理程序代码
}

LRESULT CMYTaskMangerDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1001 || wParam == 1002)
	{
		ShowWindow(SW_SHOW);
	}
	else if(wParam == 1003 || wParam == 1004)
	{
		ShowWindow(SW_HIDE);
	}
	return 0;
}