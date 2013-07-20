#ifndef __HBROWSEFOLDER__H__
#define __HBROWSEFOLDER__H__
#pragma once

class CHBrowseFolder
{
public:
	CHBrowseFolder();
	virtual ~CHBrowseFolder();
	TCHAR *GetDirPath();
	int DoModal(CWnd *pParentWnd,const TCHAR *pStartPath=NULL);
    BOOL IsValuePath(const TCHAR *pDirPath);
	static int CALLBACK BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
private:
	TCHAR *m_pDirPath;
};

#endif