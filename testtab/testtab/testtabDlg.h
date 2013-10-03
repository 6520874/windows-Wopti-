// testtabDlg.h : ͷ�ļ�
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


// CtesttabDlg �Ի���
class CYouHuaDlg : public CDialog
{
// ����
public:
	CYouHuaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YOUHUA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
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
