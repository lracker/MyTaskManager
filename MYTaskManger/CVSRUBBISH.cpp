// CVSRUBBISH.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CVSRUBBISH.h"
#include "afxdialogex.h"
#include "CTools.h"


// CVSRUBBISH 对话框

IMPLEMENT_DYNAMIC(CVSRUBBISH, CDialogEx)

CVSRUBBISH::CVSRUBBISH(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VSRubbish, pParent)
{

}

CVSRUBBISH::~CVSRUBBISH()
{
}

void CVSRUBBISH::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CVSRUBBISH, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVSRUBBISH::OnBnClickedButton1)
END_MESSAGE_MAP()


// CVSRUBBISH 消息处理程序


BOOL CVSRUBBISH::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetVsRubbish(m_CurPath, m_Paths);  //获取垃圾文件
	CRect cRect;
	GetClientRect(&cRect);
	m_List.InsertColumn(0, L"文件路径", 0, cRect.Width());
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (auto& i : m_Paths)
	{
		m_List.InsertItem(0, i);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CVSRUBBISH::OnBnClickedButton1()
{
	for (auto& i : m_Paths)
		DeleteFile(i);
	m_List.DeleteAllItems();
	m_Paths.clear();
	GetVsRubbish(m_CurPath, m_Paths);
	CRect cRect;
	GetClientRect(&cRect);
	m_List.InsertColumn(0, L"剩下的是删不掉的", 0, cRect.Width());
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (auto& i : m_Paths)
	{
		m_List.InsertItem(0, i);
	}
}
