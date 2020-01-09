// CEnumHeap.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CEnumHeap.h"
#include "afxdialogex.h"



// CEnumHeap 对话框

IMPLEMENT_DYNAMIC(CEnumHeap, CDialogEx)

CEnumHeap::CEnumHeap(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnumHeapList, pParent)
{

}

CEnumHeap::~CEnumHeap()
{
}

void CEnumHeap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CEnumHeap, CDialogEx)
END_MESSAGE_MAP()


// CEnumHeap 消息处理程序


BOOL CEnumHeap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect cRect;
	GetClientRect(cRect);
	m_List.InsertColumn(0, L"序号", 0, cRect.Width() / 4);
	m_List.InsertColumn(1, L"堆拥有者PID", 0, cRect.Width() / 4);
	m_List.InsertColumn(2, L"堆ID", 0, cRect.Width() / 4);
	m_List.InsertColumn(3, L"堆信息", 0, cRect.Width() / 4);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetHeap();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//*****************************************************************************************
// 函数名称: GetHeap
// 函数说明: 获取堆信息
// 作    者: lracker
// 时    间: 2019/09/07
// 返 回 值: bool
//*****************************************************************************************
bool CEnumHeap::GetHeap()
{
  	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, m_dwPid);
	if (hSnapShot == INVALID_HANDLE_VALUE)
		return false;
	HEAPLIST32 heap = { sizeof(HEAPLIST32) };
	if (!Heap32ListFirst(hSnapShot, &heap))
		return false;
	CString OwnerPid;
	OwnerPid.Format(_T("%d"), m_dwPid);
	int nIndex = 0;
	do 
	{	
		m_HeapList.push_back(heap);
		m_List.InsertItem(nIndex, _T(""));
		CString buffer;
		buffer.Format(_T("%d"), nIndex + 1);
		m_List.SetItemText(nIndex, 0, buffer);  //序号
		m_List.SetItemText(nIndex, 1, OwnerPid);  //堆拥有者的ID
		buffer.Format(_T("%p"), heap.th32HeapID);
		m_List.SetItemText(nIndex, 2, buffer);   //堆的ID
		if (HF32_SHARED == heap.dwFlags)
			buffer = "HF32_SHARED";
		else
			buffer = "HF32_DEFAULT";
		m_List.SetItemText(nIndex, 3, buffer);	//堆的信息
	} while (Heap32ListNext(hSnapShot,&heap));
}
