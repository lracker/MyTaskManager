// CEnumModule.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CEnumModule.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "CTools.h"


// CEnumModule 对话框

IMPLEMENT_DYNAMIC(CEnumModule, CDialogEx)

CEnumModule::CEnumModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnumModule, pParent)
{

}

CEnumModule::~CEnumModule()
{
}

void CEnumModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


bool CEnumModule::GetModule()
{
	std::vector<MODULEENTRY32> moduleList;
	getProcessAllModule(m_dwPid, &moduleList);

	// 插入到列表控件
	int index = 0;
	CString buffer;
	for (auto& i : moduleList) {
		m_List.InsertItem(index, i.szModule);

		buffer.Format(L"%08X", i.modBaseAddr);
		m_List.SetItemText(index, 1, buffer);

		buffer.Format(L"%08X", i.modBaseSize);
		m_List.SetItemText(index, 2, buffer);

		m_List.SetItemText(index, 3, i.szExePath);
	}
	return true;
}

BEGIN_MESSAGE_MAP(CEnumModule, CDialogEx)

END_MESSAGE_MAP()


// CEnumModule 消息处理程序



BOOL CEnumModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect cRect;
	GetClientRect(cRect);
	m_List.InsertColumn(0, L"模块名", 0, cRect.Width() / 4);
	m_List.InsertColumn(1, L"加载基址", 0, cRect.Width() / 4);
	m_List.InsertColumn(2, L"大小", 0, cRect.Width() / 4);
	m_List.InsertColumn(3, L"路径", 0, cRect.Width() / 4);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetModule();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
