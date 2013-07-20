// testtab.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "testtab.h"
#include "testtabDlg.h"
#include "MainDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtesttabApp

BEGIN_MESSAGE_MAP(CtesttabApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtesttabApp 构造

CtesttabApp::CtesttabApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CtesttabApp 对象

CtesttabApp theApp;


// CtesttabApp 初始化

BOOL CtesttabApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
    //skinppLoadSkin(L"Aura.ssk");
     //skinppLoadSkinFromRes(m_hInstance,NULL,NULL,);
	 //skinppRemoveSkin();
	CMainDlg dlg;
	m_pMainWnd = &dlg;

	dlg.Create(IDD_DIALOGMAIN ); //创建为非模态对话框
	//dlg.DoModal();
	dlg.ShowWindow(SW_HIDE);        //创建完毕后，可以设置对话框的显示方式，正常为“SW_SHOW”，
	//在此，我们使用“SW_HIDE”将对话框隐藏，但是在进程列表中仍然可以看到
	dlg.RunModalLoop();                  //消息循环
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
