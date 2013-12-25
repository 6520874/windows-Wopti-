#pragma once

#include <map>
#include <string>

typedef struct _PROCESS_TIME	//正在计算CPU使用的时刻
{
	bool bFirstTime;		//第一次计算
	int iProcessId;
	__int64 iSysTime;		//当前系统时间
	__int64 iProcessTime;	//进程消耗CPU的时间
	__int64 iLastSysTime;	
	__int64 iLastProcessTime;

	_PROCESS_TIME()
		: bFirstTime(true)
		, iSysTime(0)
		, iProcessId(0)
		, iProcessTime(0)
		, iLastSysTime(0)
		, iLastProcessTime(0)
	{
	}
}TProcessTime;

typedef std::map<int,TProcessTime> MAP_PROCESS_TIME;

typedef struct _CPU_MEM_INFO
{
	int			iProcessId;
	std::string	strProcessName;
	double		dbCpuUsage;		//单位:%
	double		dbMemUsage;		//单位:K

	_CPU_MEM_INFO()
		: iProcessId(0)
		, dbMemUsage(0)
		, dbCpuUsage(0)
	{
	}
}TCpuMemInfo;

typedef std::map<int,TCpuMemInfo> MAP_CPUMEM_INFO;

class CProcessConsumption
{
public:
	CProcessConsumption(void);
	~CProcessConsumption(void);

	void	Start(int iElapse=10000);
	void	Stop();

	void	GetInfoMap(MAP_CPUMEM_INFO& mapInfo);
	void	GetTotalCpuMemPercent(double& dbCpuPercent, double& dbMemPercent);
	void	GetCpuMemPercent(int iProcessId,double& dbCpuPercent, double& dbMemPercent);
	
	double	MemToPercent(double dbMemUsage);
	double	CpuToPercent(double dbCpuUsage);

private:
	void	Init();
	void	Release();
	void	InitMap();
	void	ClearMap();

	double	GetCpuUsage(int iProcessID);
	__int64	GetCurTimeAsInt64();
	__int64	GetProcessTimeAsInt64(int iProcessId);
	double	GetMemoryUsage(int iProcessId);
	__int64	FileTimeToInt64(const FILETIME& time);

	void	LockMapInfo();
	void	UnLockMapInfo();
	MAP_CPUMEM_INFO		m_mapCpuMemInfo;
	CRITICAL_SECTION	m_cirLockMapInfo;

	MAP_PROCESS_TIME	m_mapProcessTime;

	HANDLE				m_hEventStart;
	int					m_iElapse;
	bool				m_bStart;

	static void ThreadProcGetCpuMem(void* pParam);
};
