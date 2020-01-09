#pragma once
#include <vector>
using std::vector;

// CVSRUBBISH 对话框

class CVSRUBBISH : public CDialogEx
{
	DECLARE_DYNAMIC(CVSRUBBISH)

public:
	CVSRUBBISH(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVSRUBBISH();
	CString m_CurPath;
	vector<CString> m_Paths;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VSRubbish };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnBnClickedButton1();
};
