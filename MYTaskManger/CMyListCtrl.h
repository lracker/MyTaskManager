#pragma once
#include <vector>
#include <stdarg.h>
#include "CTools.h"
using std::vector;

// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	void AddColumn(DWORD dwCount, ...);
	void UpdateProList();  //�����б�
	vector<PROCESSINFO> m_NewProList;
	vector<PROCESSINFO> m_OldProList;
protected:
	DECLARE_MESSAGE_MAP()
};


