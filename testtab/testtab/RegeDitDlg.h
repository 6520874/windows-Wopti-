#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CRegeDitDlg 对话框

class CRegeDitDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegeDitDlg)
public:
	CRegeDitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegeDitDlg();

// 对话框数据
	enum { IDD = IDD_REGEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL    OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
CButton m_Button_nostartmenu;
CButton m_Buttonbigfile;
CButton  m_Button_invalidSoft;
CStatic  m_Staticscanregpath;
CListCtrl m_clearitmelist;
int  m_Ilistctrlindex; 
int  m_IvaildstartmenuNum;
CWinThread * m_pThread;
CStatic m_validMenu;
int    m_j ;

public:
	afx_msg void OnBnClickedClearreg();
	afx_msg void OnBnClickedOpenreg();
	afx_msg void OnUpdateDleteitem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenitem(CCmdUI *pCmdUI);
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStopScan();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString ReadShortcut(CString lpwLnkFile);     //读取快捷方式(com组件)
	void DisplayPath(CString   path);             //路径变量
	void DisplayCFileSize(CString   path);        //选择C盘文件大小
	void DisplayRegSoftFile();                    //选择注册表软件信息
    void CheckChoice();                           //选择清理的选项
	BOOL IsExist(HKEY hKey,LPCTSTR lpSubKey);     //是否存在
	BOOL FindSubKey(HKEY hKey,LPCTSTR lpSubKey);  //查找子项
	void DispayRegKeyPath(HKEY h1,TCHAR s[]);
	void GetKeyToValue(HKEY  h1);                 //获取一个项对应的值
    static 	UINT __cdecl Clearthread( LPVOID pParam );
private:
	CTreeCtrl m_DiskTree;
};
