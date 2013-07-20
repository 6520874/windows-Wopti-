//========================================================================================
//函数功能：
//作　　者：单鹏飞
//创建日期：2013-3-23
//参数说明：
//返回值  ：
//---------------------------------------------------------------------------------------
#include "MainDlg.h"
#pragma once
class ComputerExamineUI : public CContainerUI,public INotifyUI
{

private:
	CButtonUI* log_button;

public:
	ComputerExamineUI()
	{
		CDialogBuilder builder;
                                                                                     
		//CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("Main.xml"), (UINT)0));

		/*if(pComputerExamine )
		{
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}*/

	}

	virtual  void Notify(TNotifyUI& msg)
	{
		CDuiString name = msg.pSender->GetName();
		if(name == _T("ButtonUI3"))
		{
			//CMainDlg::OnSysCommand();
			//CMainDlg::Instance()->DoOpenClick();
		}
		
	}

	virtual LPCTSTR GetWindowClassName() const
	{

		return NULL;
	}



};


class  ClearToolbar: public CContainerUI,public INotifyUI
{
public:
	ClearToolbar()
	{
		CDialogBuilder builder;
		/*CContainerUI* pClearToolbar = static_cast<CContainerUI*>(builder.Create(_T("ClearToolbar.xml"), (UINT)0));

		if( pClearToolbar )
		{
			this->Add(pClearToolbar);
		}
		else {
			this->RemoveAll();
			return;
		}*/
	}


	void OnPrepare()
	{
		

	}

	virtual  void Notify(TNotifyUI& msg)
	{
		CDuiString name = msg.pSender->GetName();

		if( msg.sType == _T("windowinit") ) 
		{
			OnPrepare();
		}

	     if(name==_T("ListUITaskMgr"))
		{

		}

		 if(name == _T("ButtonUI3"))
		 {

		 }
		
		
	}

};

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("ComputerExamine")) == 0 ) return new ComputerExamineUI;

		if(_tcscmp(pstrClass,_T("ClearToolbar")) ==0) return new ClearToolbar;

		return NULL;
	}
};