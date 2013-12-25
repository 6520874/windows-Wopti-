#include "StdAfx.h"
#include "ProcessConsumption.h"

#include "tlhelp32.h"   
#include "Psapi.h"
#include <process.h>

#pragma  comment(lib,"Psapi.lib")


CProcessConsumption::CProcessConsumption(void)
	: m_hEventStart(NULL)
	, m_iElapse(10000)
	, m_bStart(false)
{
	Init();
}

CProcessConsumption::~CProcessConsumption(void)
{
	Release();
}

void CProcessConsumption::Start(int iElapse/*=10000*/)
{
	if (m_bStart)
	{
		return;
	}

	if (iElapse < 1000)
	{
		iElapse = 1000;
	}
	m_iElapse = iElapse;

	if (m_hEventStart != NULL)
	{
		SetEvent(m_hEventStart);
	}

	_beginthread(CProcessConsumption::ThreadProcGetCpuMem,0,this);
	
	m_bStart = true;
}

void CProcessConsumption::Stop()
{
	if (m_hEventStart != NULL)
	{
		ResetEvent(m_hEventStart);
	}

	ClearMap();

	m_bStart = false;
}

void CProcessConsumption::GetInfoMap( MAP_CPUMEM_INFO& mapInfo )
{
	LockMapInfo();
	mapInfo = m_mapCpuMemInfo;
	UnLockMapInfo();
}

void CProcessConsumption::GetTotalCpuMemPercent(double& dbCpuPercent, double& dbMemPercent)
{
	double dbCpuUsage = 0;
	double dbMemUsage = 0;

	MAP_CPUMEM_INFO mapInfo;
	GetInfoMap(mapInfo);
	for (MAP_CPUMEM_INFO::iterator it=mapInfo.begin(); it!=mapInfo.end(); ++it)
	{
		dbCpuUsage += it->second.dbCpuUsage;
		dbMemUsage += it->second.dbMemUsage;
	}

	dbCpuPercent = CpuToPercent(dbCpuUsage);
	dbMemPercent = MemToPercent(dbMemUsage);
}

void CProcessConsumption::GetCpuMemPercent(int iProcessId,double& dbCpuPercent, double& dbMemPercent)
{
	double dbCpuUsage = 0;
	double dbMemUsage = 0;

	MAP_CPUMEM_INFO mapInfo;
	GetInfoMap(mapInfo);
	MAP_CPUMEM_INFO::iterator it = mapInfo.find(iProcessId);
	if (it != mapInfo.end())
	{
		dbCpuUsage = it->second.dbCpuUsage;
		dbMemUsage = it->second.dbMemUsage;
	}

	dbCpuPercent = CpuToPercent(dbCpuUsage);
	dbMemPercent = MemToPercent(dbMemUsage);
}

double CProcessConsumption::MemToPercent(double dbMemUsage)
{
	if (dbMemUsage < 0)
	{
		return 0;
	}

	double dbMemPercent = 0;

	MEMORYSTATUS MemoryStatus;
	GlobalMemoryStatus(&MemoryStatus); 
	double dbMemTotal = MemoryStatus.dwTotalPhys/1024;
	dbMemPercent = dbMemUsage * 100 / dbMemTotal;

	if (dbMemPercent > 100)
	{
		dbMemPercent = 100;
	}

	return dbMemPercent;
}

double CProcessConsumption::CpuToPercent(double dbCpuUsage)
{
	if (dbCpuUsage < 0)
	{
		dbCpuUsage = 0;
	}
	else if (dbCpuUsage > 100)
	{
		dbCpuUsage = 100;
	}

	return dbCpuUsage;
}

void CProcessConsumption::Init()
{
	m_hEventStart = CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&m_cirLockMapInfo);	
}

void CProcessConsumption::Release()
{
	if (m_bStart)
	{
		Stop();
	}

	if (m_hEventStart != NULL)
	{
		CloseHandle(m_hEventStart);
		m_hEventStart = NULL;
	}

	ClearMap();
	DeleteCriticalSection(&m_cirLockMapInfo);
}

void CProcessConsumption::InitMap()
{
	PROCESSENTRY32 pe;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hSnapshot,&pe))
	{
		CloseHandle(hSnapshot);
		return;
	}

	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if(!Process32Next(hSnapshot,&pe)) 
		{
			break;
		}

		LockMapInfo();
		TCpuMemInfo* pInfo = NULL;
		MAP_CPUMEM_INFO::iterator it = m_mapCpuMemInfo.find(pe.th32ProcessID);
		if (it != m_mapCpuMemInfo.end())
		{
			TCpuMemInfo& info = it->second;
			info.iProcessId = pe.th32ProcessID;
			info.strProcessName = CT2A(pe.szExeFile);
			m_mapCpuMemInfo[info.iProcessId] = info;
		}
		else
		{
			TCpuMemInfo info;
			info.iProcessId = pe.th32ProcessID;
			info.strProcessName = CT2A(pe.szExeFile);
			m_mapCpuMemInfo[info.iProcessId] = info;
		}	
		UnLockMapInfo();
	}
	while(true);

	CloseHandle(hSnapshot);
}

void CProcessConsumption::ClearMap()
{
	LockMapInfo();
	m_mapCpuMemInfo.clear();
	UnLockMapInfo();
}

double CProcessConsumption::GetCpuUsage(int iProcessID)
{
	static int				s_iCpuCount = 0;		//cpu数量
	if (s_iCpuCount <= 0)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		s_iCpuCount = info.dwNumberOfProcessors;
	}

	MAP_PROCESS_TIME::iterator it = m_mapProcessTime.find(iProcessID);
	if (it != m_mapProcessTime.end())
	{		
		TProcessTime& time = it->second;
		if (!time.bFirstTime)
		{
			time.iLastSysTime = time.iSysTime;
			time.iLastProcessTime = time.iProcessTime;
			time.iSysTime = GetCurTimeAsInt64();
			time.iProcessTime = GetProcessTimeAsInt64(iProcessID);

			__int64 iTimeElapse = time.iSysTime - time.iLastSysTime;
			__int64 iTimeProElapse = time.iProcessTime - time.iLastProcessTime;
			if (iTimeElapse>0 && iTimeProElapse>=0 && s_iCpuCount>0)
			{
				double dbCpuUsage = (double)(100*iTimeProElapse)/iTimeElapse/s_iCpuCount;
				return dbCpuUsage;
			}
		}
	}
	else
	{
		TProcessTime time;
		time.iProcessId = iProcessID;
		time.iSysTime = GetCurTimeAsInt64();
		time.iProcessTime = GetProcessTimeAsInt64(iProcessID);
		time.bFirstTime = false;
		m_mapProcessTime[iProcessID] = time;
	}

	return 0;
}

__int64	CProcessConsumption::GetCurTimeAsInt64()
{
	FILETIME time;
	GetSystemTimeAsFileTime(&time);
	return FileTimeToInt64(time);
}

__int64	CProcessConsumption::GetProcessTimeAsInt64(int iProcessId)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, iProcessId);
	if (!hProcess)
	{
		return 0;
	}

	FILETIME timeCreate;	//进程的创建时间
	FILETIME timeTimeExit;	//进程的退出时间
	FILETIME timeKernel;	//进程在内核模式下的所有时间
	FILETIME timeUser;		//进程在用户模式下的所有时间
	if (!GetProcessTimes(hProcess, &timeCreate, &timeTimeExit, &timeKernel, &timeUser))
	{
		CloseHandle(hProcess);
		return 0;
	}
   
	CloseHandle(hProcess);
	return FileTimeToInt64(timeKernel)+FileTimeToInt64(timeUser);
}

double CProcessConsumption::GetMemoryUsage(int iProcessId)
{
	HANDLE hProcess = NULL;
	PROCESS_MEMORY_COUNTERS pmc;
	int iProcessMemoryInK = 0;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE, iProcessId);
	if (NULL == hProcess)
	{
		return -1;
	}

	if(0 == GetPriorityClass(hProcess))
	{
		CloseHandle( hProcess );
		return -1;
	}

	if (GetProcessMemoryInfo(hProcess,&pmc,sizeof(pmc)) )
	{
		iProcessMemoryInK = pmc.WorkingSetSize/1024;		//单位为k  
	}

	CloseHandle( hProcess );

	return iProcessMemoryInK;
}

__int64	CProcessConsumption::FileTimeToInt64(const FILETIME& time)
{
	ULARGE_INTEGER tt;
	tt.LowPart = time.dwLowDateTime;
	tt.HighPart = time.dwHighDateTime;

	return tt.QuadPart;
}

void CProcessConsumption::LockMapInfo()
{
	EnterCriticalSection(&m_cirLockMapInfo);
}

void CProcessConsumption::UnLockMapInfo()
{
	LeaveCriticalSection(&m_cirLockMapInfo);
}

void CProcessConsumption::ThreadProcGetCpuMem( void* pParam )
{
	CProcessConsumption* pInstance = (CProcessConsumption*)pParam;
	if (NULL == pInstance)
	{
		return;
	}

	while (WAIT_OBJECT_0 == WaitForSingleObject(pInstance->m_hEventStart,0))
	{
		pInstance->InitMap();
		pInstance->LockMapInfo();
		for (MAP_CPUMEM_INFO::iterator it=pInstance->m_mapCpuMemInfo.begin(); it!=pInstance->m_mapCpuMemInfo.end(); )
		{
			TCpuMemInfo& info = it->second;
			info.dbCpuUsage = pInstance->GetCpuUsage(info.iProcessId);
			info.dbMemUsage = pInstance->GetMemoryUsage(info.iProcessId);

			if (info.dbCpuUsage<=0 && info.dbMemUsage<100)
			{
				//删除未运行的进程。
				it = pInstance->m_mapCpuMemInfo.erase(it);
			}
			else
			{
				info.dbCpuUsage		= info.dbCpuUsage;
				info.dbMemUsage		= info.dbMemUsage;
				++it;
			}
		}
		pInstance->UnLockMapInfo();

		if (WAIT_OBJECT_0 == WaitForSingleObject(pInstance->m_hEventStart,0))
		{
			Sleep(pInstance->m_iElapse);
		}
	}
}
