#pragma once
#include <TlHelp32.h>
#include <vector>
using std::vector;
// CEnumThread 对话框

class CEnumThread : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumThread)

public:
	CEnumThread(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumThread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EnumThread };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool GetThread();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	DWORD m_dwPid;

	DWORD m_dwSelect;
	vector<THREADENTRY32> m_ThreadList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
