#pragma once
#include "afxcmn.h"


// CFileView 对话框

class CFileView : public CDialog
{
	DECLARE_DYNAMIC(CFileView)

public:
	CFileView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileView();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_tree;
public:
	afx_msg void OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void InitDriverTree();
	void InsertNode(CString szPath, HTREEITEM hNode);
	CString GetFullPath(HTREEITEM hNode);
    afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnUpdateDleteitem(CCmdUI *pCmdUI);
    };
