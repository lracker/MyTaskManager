#pragma once


// CFileTree

class CFileTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFileTree)

public:
	CFileTree();
	virtual ~CFileTree();
	void InitTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
};


