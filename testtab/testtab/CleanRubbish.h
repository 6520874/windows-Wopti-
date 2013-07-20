#pragma once
#include "afxwin.h"


// CCleanRubbish 对话框

class CCleanRubbish : public CDialog
{
	DECLARE_DYNAMIC(CCleanRubbish)

public:
	CCleanRubbish(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCleanRubbish();

// 对话框数据
	enum { IDD = IDD_DIALOGCLEANRUBBISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonstartscan();
	void SelectChoice();
	CButton m_chekTemp;
	CButton m_checklog;
	CButton m_checkgid;
	CButton m_checkold;
	afx_msg void OnBnClickedButtonStopscan();
	CWinThread  * m_Windlg;
};
