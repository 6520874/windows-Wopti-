#include "stdafx.h"
#include "360Safe.h"

C360SafeUI::C360SafeUI()
{

}


LPCTSTR C360SafeUI::GetWindowClassName() const
{
	return _T("UIMainFrame");
}

void C360SafeUI::Notify( TNotifyUI& msg )
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") )
	{
        	  CMainDlg d;

		if(msg.pSender == m_PYouhua)
		{
			  d.OnBnClickedButtonYouhua();
		}
         
		if(msg.pSender == m_pMeihua)
		{
			d.OnBnClickedButtonMeihua();
		}
        
		if(msg.pSender == m_PClear)
		{
			d.OnBnClickedButtonClear();
		}

		if( msg.pSender == m_pCloseBtn ) 
		{
			PostQuitMessage(0);
			return; 
		}
		else if( msg.pSender == m_pMinBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
		else if( msg.pSender == m_pMaxBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
		else if( msg.pSender == m_pRestoreBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;}

	}

	else if(msg.sType==_T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
 
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
		if(name==_T("examine"))
			pControl->SelectItem(0);

		else if(name==_T("trojan"))
		{
			pControl->SelectItem(1);
		}
		else if(name==_T("plugins"))
			pControl->SelectItem(2);

		else if(name==_T("vulnerability"))
			pControl->SelectItem(3);

		else if(name==_T("rubbish"))
			pControl->SelectItem(4);

		else if(name==_T("cleanup"))
			pControl->SelectItem(5);

		else if(name==_T("fix"))
			pControl->SelectItem(6);

		else if(name==_T("tool"))
			pControl->SelectItem(7);


	}
}

LRESULT C360SafeUI::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	Init();

	ClearToolbar   *pClearToolbar = new ClearToolbar;
	m_pm.AddNotifier(pClearToolbar);
	/*CListUI  *list  = static_cast<CListUI*>(m_pm.FindControl(L"ListUITaskMgr"));

	for(int j=0;j<5;j++)
	{
		CListTextElementUI  *pListElement = new CListTextElementUI;
		pListElement->SetOwner(list);
		for(int i=0;i<5;i++)
		{
			pListElement->SetText(i,L"测试测试");
		}

		list->AddAt(pListElement,j);
	}*/
	return 0;
}




LRESULT C360SafeUI::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT C360SafeUI::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT C360SafeUI::OnNcActivate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT C360SafeUI::OnNcCalcSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT C360SafeUI::OnNcPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT C360SafeUI::OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_pm.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT C360SafeUI::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT C360SafeUI::OnGetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT C360SafeUI::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT C360SafeUI::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;

	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_NOTIFY:        
	default:
		bHandled = FALSE;
	}

	if( bHandled ) return lRes;

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	CWindowWnd;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}





void C360SafeUI::Init()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	m_PYouhua = static_cast<COptionUI*>(m_pm.FindControl(L"YouhuaMaster"));
    m_PClear = static_cast<COptionUI*>(m_pm.FindControl(L"ClearMaster"));
	m_pMeihua = static_cast<COptionUI*>(m_pm.FindControl(L"MeihuaMaster"));
	RECT rcAdress={182,180,534,204};

}

void C360SafeUI::OnFinalMessage( HWND /*hWnd*/ )
{
	delete this;
}

UINT C360SafeUI::GetClassStyle() const
{
	return CS_DBLCLKS;
}

void C360SafeUI::OnPrepare()
{

}

void C360SafeUI::LoadSkin()
{
	LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(m_hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowText(m_hWnd, _T("360界面"));
	m_pm.Init(m_hWnd);

	CDialogBuilder builder;
	if (m_pm.GetResourceZip().IsEmpty())
	{                                                                           
		HRSRC hResource = ::FindResource(m_pm.GetResourceDll(), MAKEINTRESOURCE(IDR_360SAFEZIP), _T("ZIP"));
		if( hResource == NULL )
		{ 
			AfxMessageBox(L"没有资源");
			TRACE("can't find hResource");          
			return;
		}

		DWORD dwSize = 0;

		HGLOBAL hGlobal = ::LoadResource(m_pm.GetResourceDll(), hResource);
		if( hGlobal == NULL )
		{
			TRACE("can't LoadResource");          
			FreeResource(hResource);
			return;
		}

		dwSize = ::SizeofResource(m_pm.GetResourceDll(), hResource);
		if( dwSize == 0 )
			return;

		LPBYTE m_zipBuffer = new BYTE[dwSize];

		if (m_zipBuffer != NULL)
		{
			::CopyMemory(m_zipBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
		}

		::FreeResource(hResource);
		m_pm.SetResourceZip(m_zipBuffer, dwSize);
	}

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return ;

	C360SafeUI* pFrame = new C360SafeUI();
	if( pFrame == NULL ) return ;

	pFrame->Create(NULL, _T("windows管理软件"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	::CoUninitialize();
	

}

