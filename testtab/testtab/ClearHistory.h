#pragma once
#include "afxwin.h"


// CClearHistory �Ի���

class CClearHistory : public CDialog
{
	DECLARE_DYNAMIC(CClearHistory)

public:
	CClearHistory(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClearHistory();

	enum DEL_CACHE_TYPE //Ҫɾ�������͡�
	{
		File,//��ʾinternet��ʱ�ļ�
		Cookie //��ʾCookie
	};

// �Ի�������
	enum { IDD = IDD_DIALOG_CLEARHISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
