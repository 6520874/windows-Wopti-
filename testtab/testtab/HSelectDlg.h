#pragma once
#include "afxwin.h"


// CHSelectDlg �Ի���

class CHSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CHSelectDlg)

public:
	CHSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHSelectDlg();

// �Ի�������
	enum { IDD = IDD_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
