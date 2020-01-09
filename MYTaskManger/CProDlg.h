#pragma once
#include "CMyListCtrl.h"

// CProDia 对话框

class CProDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProDlg)

public:
	CProDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ProDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_MyListCtrl;
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
	DWORD m_dwPid;
	CMenu m_Menu;
	CStatusBar* m_StatusBar;
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetThread();
	afx_msg void OnGetModule();
	afx_msg void OnEnumHeap();
protected:
	afx_msg LRESULT OnCpu(WPARAM wParam, LPARAM lParam);
};
