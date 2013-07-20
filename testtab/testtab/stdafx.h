// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ 
#include <afx.h>
#include <afxdisp.h>        // MFC �Զ�����


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>
#include "Resource.h"
#include "psapi.h"
#include "UIlib.h"

#include <afxdhtml.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib,"version.lib")


#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "lib/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "lib/DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "lib/DuiLib_u.lib")
#   else
#       pragma comment(lib, "lib/DuiLib.lib")
#   endif
#endif

typedef struct  
{
	DWORD	dwProID;			    //����ID
	DWORD   dwParentProID;         //������ID 
	TCHAR	szExeFile[100];	       //������
	DWORD	cntThreads;			    //�߳���
	DWORD	dwMemoryUse;			//����ʹ�õ��ڴ棬��KBΪ��λ
	TCHAR    szProCompany[256];     //�ļ�����
}PROCITEM_S;


struct LANGANDCODEPAGE 
{
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;


/*
typedef LONG NTSTATUS;
typedef LONG KPRIORITY;


#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define STATUS_INFO_LENGTH_MISMATCH    ((NTSTATUS)0xC0000004L)

#define SystemProcessesAndThreadsInformation 5

typedef struct _CLIENT_ID {
	DWORD        UniqueProcess;
	DWORD        UniqueThread;
}
CLIENT_ID;

typedef struct _UNICODE_STRING {
	USHORT        Length;
	USHORT        MaximumLength;
	PWSTR        Buffer;
}
UNICODE_STRING;

typedef struct _VM_COUNTERS {
	SIZE_T        PeakVirtualSize;
	SIZE_T        VirtualSize;
	ULONG        PageFaultCount;
	SIZE_T        PeakWorkingSetSize;
	SIZE_T        WorkingSetSize;
	SIZE_T        QuotaPeakPagedPoolUsage;
	SIZE_T        QuotaPagedPoolUsage;
	SIZE_T        QuotaPeakNonPagedPoolUsage;
	SIZE_T        QuotaNonPagedPoolUsage;
	SIZE_T        PagefileUsage;
	SIZE_T        PeakPagefileUsage;
}
VM_COUNTERS;

typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID        ClientId;
	KPRIORITY        Priority;
	KPRIORITY        BasePriority;
	ULONG ContextSwitchCount;
	LONG State;
	LONG WaitReason;
}
SYSTEM_THREADS, * PSYSTEM_THREADS;

// Note that the size of the SYSTEM_PROCESSES structure is different on
// NT 4 and Win2K, but we don��t care about it, since we don��t access neither
// IoCounters member nor Threads array

typedef struct _SYSTEM_PROCESSES {
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING   ProcessName;
	KPRIORITY        BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS        VmCounters;
#if _WIN32_WINNT >= 0x00
	IO_COUNTERS        IoCounters;
#endif
	SYSTEM_THREADS   Threads[1];
}
SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

*/


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>
#include <atltypes.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

BOOL MultiByteToUnicode(LPCSTR pInput, int nInput, LPTSTR pOutput, int& nOutput);
BOOL UnicodeToMultiByte(LPCTSTR pInput, int nInput, LPSTR pOutput, int& nOutput);


#define WM_AFX_TEST1_MSG WM_USER + 103
#define WM_AFX_TEST2_MSG WM_USER + 101
#define WM_AFX_HELLO_MSG WM_USER + 102


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


