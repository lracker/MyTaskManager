// CFileTree.cpp: 实现文件
//

#include "pch.h"
#include "MYTaskManger.h"
#include "CFileTree.h"
#include <strsafe.h>


// CFileTree

IMPLEMENT_DYNAMIC(CFileTree, CTreeCtrl)

CFileTree::CFileTree()
{

}

CFileTree::~CFileTree()
{
}


void CFileTree::InitTree()
{
	WCHAR szName[MAX_PATH] = { 0 };
	GetLogicalDriveStrings(MAX_PATH, szName);
	for (int i = 0; szName[i] != '\0' && i < MAX_PATH; i++)
	{
		HTREEITEM hItem = InsertItem(&szName[i]);   //插入盘符
		wchar_t* pBuff = _wcsdup(&szName[i]);
		SetItemData(hItem, (DWORD_PTR)pBuff);
		i += wcslen(&szName[i]);  
	}
}

BEGIN_MESSAGE_MAP(CFileTree, CTreeCtrl)
END_MESSAGE_MAP()



// CFileTree 消息处理程序


