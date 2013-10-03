// testtabDlg.h : 头文件
//
#include "CTaskMgr.h"
#include "CNetWork.h"
#include "RegeDitDlg.h"
#include "FileExplorer.h"
#include "StartMag.h"
#include "HSelectDlg.h"
#include "SystemInfoDlg.h"

#include "FileView.h"
#pragma once


// CtesttabDlg 对话框
class CYouHuaDlg : public CDialog
{
// 构造
public:
	CYouHuaDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YOUHUA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabctr;
	CTaskMgrDlg   m_para1;
	CNetWorkDlg   m_para2;
	CFileExplorer m_para3;
	CStartMagDlg  m_para4;
	CRegeDitDlg   m_para5;
    CFileView     m_para6;
	CImageList    m_ImageTab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
