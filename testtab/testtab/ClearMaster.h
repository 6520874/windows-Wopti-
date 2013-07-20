#pragma once
#include "afxcmn.h"
#include "RegeDitDlg.h"
#include "ClearHistory.h"
#include "CleanRubbish.h"

// CClearMaster �Ի���

class CClearMaster : public CDialog
{
	DECLARE_DYNAMIC(CClearMaster)

public:
	CClearMaster(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClearMaster();

// �Ի�������
	enum { IDD = IDD_CLEARMASTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabClear;
    CCleanRubbish m_Rubbish;
	CRegeDitDlg   m_RegeditDlg;
	CClearHistory m_ClearHistoryDlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
