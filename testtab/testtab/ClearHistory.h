#pragma once
#include "afxwin.h"


// CClearHistory 对话框

class CClearHistory : public CDialog
{
	DECLARE_DYNAMIC(CClearHistory)

public:
	CClearHistory(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClearHistory();

	enum DEL_CACHE_TYPE //要删除的类型。
	{
		File,//表示internet临时文件
		Cookie //表示Cookie
	};

// 对话框数据
	enum { IDD = IDD_DIALOG_CLEARHISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartclear();
	BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
  BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,   BOOL bWipeIndexDat = FALSE);
	BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);
	CButton m_CheckInternet;  //
	CButton m_checkClearCookies;
	CButton m_CheckBrowserClear;
	CButton m_chekClearPasswd;
	CButton m_checkClearRecyle;
	CButton m_checkClearRunCommand;
	CButton m_checkBrowserHistory;
	afx_msg void OnBnClickedButtonEndclear();
};
