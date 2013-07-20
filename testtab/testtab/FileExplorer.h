#pragma once
#include "afxcmn.h"

#include <vector>
using namespace std;
// CFileExplorer 对话框
typedef struct tagCheckInfo
{
	TCHAR szIndex[MAX_PATH];
	TCHAR szTime[MAX_PATH];
	TCHAR szType[MAX_PATH];
	TCHAR szDetail[1024];
}CheckInfo,*pCheckInfo;

class CFileExplorer : public CDialog
{
	DECLARE_DYNAMIC(CFileExplorer)

public:
	CFileExplorer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileExplorer();

// 对话框数据
	enum { IDD = IDD_FILEEXPLORER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	static DWORD WINAPI ThreadWatchProc( LPVOID lParam );
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedStartmonitor();
	afx_msg void OnBnClickedButtonOption();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStop();
	BOOL  StartWatch( CString strPath );

	//StartWatch( CString strPath )
private:
	CListCtrl m_listen;
	CFont    m_hFont;
    typedef vector<pCheckInfo>          m_vectorcheckinfo;
	m_vectorcheckinfo                   m_vecCI;
	BOOL								m_bAddNew;
	BOOL								m_bDelete;
	BOOL								m_bRename;
	BOOL								m_bModify;
	BOOL								m_bOther;
	BOOL								m_bAlwaysOnTop;
	BOOL								m_bAutoRun;
	BOOL								m_bMinimized;
	BOOL								m_bAutoStart;
	int									m_iListCount;
	TCHAR							    *m_pszListIndex;
	HANDLE								m_hWatchThread;
	HANDLE								m_hDirectory;
	HICON								m_hIcon;
	CString                             m_strWatchedDir;  

public:
	afx_msg void OnBnClickedButtonChoicepath();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClearall();
	afx_msg void OnNMClickListen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedCheck1();
};
