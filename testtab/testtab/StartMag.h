#pragma once
#include "afxcmn.h"


// CStartMag 对话框

class CStartMagDlg : public CDialog
{
	DECLARE_DYNAMIC(CStartMagDlg)

public:
	CStartMagDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartMagDlg();

// 对话框数据
	enum { IDD = IDD_STARTMANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void GetRunRegpath();

private:
	CListCtrl m_startItemList;
};
