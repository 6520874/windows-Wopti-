// ClearHistory.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "ClearHistory.h"
#include <UrlHist.h>
#include <wininet.h>
#define SWEEP_BUFFER_SIZE			10000
// CClearHistory 对话框

IMPLEMENT_DYNAMIC(CClearHistory, CDialog)

CClearHistory::CClearHistory(CWnd* pParent /*=NULL*/)
	: CDialog(CClearHistory::IDD, pParent)
{

}

CClearHistory::~CClearHistory()
{
}

void CClearHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CLEARINTERNET, m_CheckInternet);
	DDX_Control(pDX, IDC_CHECK_CLEARCOOKIES, m_checkClearCookies);
	DDX_Control(pDX, IDC_CHECK_CLEARBWOSER, m_CheckBrowserClear);
	DDX_Control(pDX, IDC_CHECK5, m_chekClearPasswd);
	DDX_Control(pDX, IDC_CHECK6, m_checkClearRecyle);
	DDX_Control(pDX, IDC_CHECK7, m_checkClearRunCommand);
	DDX_Control(pDX, IDC_CHECK8, m_checkBrowserHistory);
}


BEGIN_MESSAGE_MAP(CClearHistory, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_STARTCLEAR, &CClearHistory::OnBnClickedButtonStartclear)
	ON_BN_CLICKED(IDC_BUTTON_ENDCLEAR, &CClearHistory::OnBnClickedButtonEndclear)
END_MESSAGE_MAP()


// CClearHistory 消息处理程序


BOOL CClearHistory::DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}

	do
	{
		if (type == File &&
			!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == Cookie &&
			(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}

		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete [] lpCacheEntry; 
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	}
	while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete [] lpCacheEntry; 
	}
	return bRet;
}


BOOL CClearHistory::EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, 
									  BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else
		{
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, _T("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			DeleteFile(sFullPath);
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	return TRUE;
}


BOOL CClearHistory::WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	char	sZero[256];
	memset(sZero, 0, 256);

	sPath = szDir;
	sPath += _T('\\');
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwSize = GetFileSize(hFile, NULL);

	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}

	CloseHandle(hFile);
	return TRUE;
}






void CClearHistory::OnBnClickedButtonStartclear()
{
	// TODO: 在此添加控件通知处理程序代码

	// //清internet临时文件
    if(m_CheckInternet.GetCheck())
	{
		TCHAR szPath[MAX_PATH];
		DeleteUrlCache(File);
		if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
		{  //得到临时目录，并清空它.
		EmptyDirectory(szPath);
		}

	}
   

	if(m_checkClearCookies.GetCheck())
	{
 	//Cookie的清除

	TCHAR szPath[MAX_PATH];	
	DeleteUrlCache(Cookie);
		if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
		{//得到目录，并清空
			EmptyDirectory(szPath);
		}
	} 

	if(m_CheckBrowserClear.GetCheck())
	{
		//浏览器地址栏历史地址的清除
		SHDeleteKey(HKEY_CURRENT_USER, 
			_T("Software\\Microsoft\\Internet Explorer\\TypedURLs"));

	}

	if(m_checkBrowserHistory.GetCheck())
	{
		// 清浏览网址历史记录
		HRESULT hr;
		TCHAR szPath[MAX_PATH];	
		IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
		hr = CoCreateInstance(CLSID_CUrlHistory, NULL, 
			CLSCTX_INPROC_SERVER, IID_IUrlHistoryStg2, 
			(void**)&pUrlHistoryStg2);
		if (SUCCEEDED(hr))
		{
			hr = pUrlHistoryStg2->ClearHistory(); 
			pUrlHistoryStg2->Release();
		}

		// 如果上面代码不能清
		// 则有下面的，不完美，但能工作.
		GetWindowsDirectory(szPath, MAX_PATH);
		_tcscat(szPath, _T("\\History"));
		EmptyDirectory(szPath, FALSE, TRUE);

		if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_HISTORY, FALSE))
		{
			EmptyDirectory(szPath, FALSE, TRUE);
		}
	}

	if(m_checkClearRecyle.GetCheck())  // 清空回收站
	{
		SHEmptyRecycleBin(NULL, NULL, 
			SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
	}

	if(m_checkClearRunCommand.GetCheck())
	{
		// 清除"运行"中的自动匹配历史记录
		SHDeleteKey(HKEY_CURRENT_USER, 
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));
	}

}


void CClearHistory::OnBnClickedButtonEndclear()
{
	// TODO: 在此添加控件通知处理程序代码

}
