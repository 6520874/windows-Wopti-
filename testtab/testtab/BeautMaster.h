#pragma once
#include "afxcmn.h"
#include "UsbShaDuDlg.h"
// CBeautMaster 对话框

class CBeautMaster : public CDialog
{
	DECLARE_DYNAMIC(CBeautMaster)

public:
	CBeautMaster(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBeautMaster();

// 对话框数据
	enum { IDD = IDD_MEIHUA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabXiuFu;
    CUsbShaDuDlg m_USBCleanDlg;
	virtual BOOL OnInitDialog();
};
