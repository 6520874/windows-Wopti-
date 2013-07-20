#pragma once
#include "afxwin.h"


// CHSelectDlg 对话框

class CHSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CHSelectDlg)

public:
	CHSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHSelectDlg();

// 对话框数据
	enum { IDD = IDD_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectall();
    afx_msg void OnBnClickedButtonReverselect();
private:
	CButton m_selectall;
	CButton m_reselectall;
	CButton m_addnew;
	CButton m_deletefile;
	CButton m_renamefile;
	CButton m_modifyfile;
	CButton m_othersfile;

	
};
