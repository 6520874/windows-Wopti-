// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// testtab.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"



BOOL MultiByteToUnicode(LPCSTR pInput, int nInput, LPTSTR pOutput, int& nOutput)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, pInput, -1, NULL, 0);
	if(nSize > nOutput)
		return FALSE;

	nOutput = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, pInput, nInput, pOutput, nSize);
	return TRUE;
}

BOOL UnicodeToMultiByte(LPCTSTR pInput, int nInput, LPSTR pOutput, int& nOutput)
{
	int nSize = WideCharToMultiByte(CP_ACP, 0, pInput, -1,NULL, 0, NULL, NULL);
	if(nSize > nOutput)
		return FALSE;
	nOutput = WideCharToMultiByte(CP_ACP, 0, pInput, nInput, pOutput, nSize, NULL, 0);
	return TRUE;
}