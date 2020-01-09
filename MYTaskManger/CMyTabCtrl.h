#pragma once
// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();
	void AddTab(DWORD dwCount, ...);
	void AddDlg(DWORD dwCount, ...);
	void SelectAndShow(int nSel);
	CDialogEx* m_Dlg[20];
	DWORD m_dwCount;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
};


