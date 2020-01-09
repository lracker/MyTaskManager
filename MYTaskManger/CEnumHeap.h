#pragma once
#include <vector>
using std::vector;
#include <TlHelp32.h>
// CEnumHeap 对话框

class CEnumHeap : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumHeap)

public:
	CEnumHeap(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumHeap();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EnumHeapList };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	DWORD m_dwPid;
	virtual BOOL OnInitDialog();
	bool GetHeap();
	vector<HEAPLIST32> m_HeapList;
};
