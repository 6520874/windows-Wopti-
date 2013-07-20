#include "StdAfx.h"
#include "HBrowseFolder.h"

int CALLBACK CHBrowseFolder::BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	CHBrowseFolder *pBrowseFolder=(CHBrowseFolder *)lpData;


	if(uMsg == BFFM_INITIALIZED)
		SendMessage(hWnd,BFFM_SETSELECTION,TRUE,(LPARAM) pBrowseFolder->m_pDirPath);
	return 0;
}

CHBrowseFolder::CHBrowseFolder()
{
	m_pDirPath = new TCHAR[MAX_PATH];
}

CHBrowseFolder::~CHBrowseFolder()
{
	delete m_pDirPath;
}

int CHBrowseFolder::DoModal(CWnd *pParentWnd,const TCHAR *pStartPath)
{
	LPMALLOC pMalloc;
	if(SHGetMalloc(&pMalloc)!=NOERROR)
		return -1;
	if(pStartPath && IsValuePath(pStartPath))
	{
		TCHAR *pStr = m_pDirPath;
		while(*pStr++=*pStartPath++);
	}
	BROWSEINFO bInfo={pParentWnd->m_hWnd,NULL,0,_T("ÇëÑ¡ÔñÎÄ¼þ¼Ð:"),0,BrowseCallbackProc,(LPARAM) this,0};
	ITEMIDLIST *pItemList=SHBrowseForFolder(&bInfo);
	if(pItemList)
	{
		SHGetPathFromIDList(pItemList,m_pDirPath);
		pMalloc->Free(pItemList);
		pMalloc->Release();
		return TRUE;
	}
	pMalloc->Free(pItemList);
	pMalloc->Release();
	return FALSE;
}

TCHAR *CHBrowseFolder::GetDirPath()
{
	return m_pDirPath;
}

BOOL CHBrowseFolder::IsValuePath(const TCHAR *pDirPath)
{
	DWORD dFileAttrib=GetFileAttributes(pDirPath);
	return (dFileAttrib!=0xffffffff && dFileAttrib & FILE_ATTRIBUTE_DIRECTORY)?TRUE:FALSE;
}