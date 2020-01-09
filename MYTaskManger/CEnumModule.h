#pragma once
#include <vector>
#include "psapi.h"
using std::vector;

// CEnumModule 对话框

class CEnumModule : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumModule)

public:
	CEnumModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumModule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EnumModule };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool GetModule();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	vector<MODULEINFO> ModuleList;
	DWORD m_dwPid;
};
