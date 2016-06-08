#include "stdafx.h"
#include "Inspection.h"
#include "Utility.h"

HMODULE g_hJitModule = NULL;
HMODULE g_hClrModule = NULL;

DotNetVersion Inspection::g_tDotNetVersion;
HANDLE Inspection::s_hEvent = ::CreateEventW( NULL, TRUE, FALSE, NULL);
INT Inspection::s_nStatus = Inspection::Status_Uninitialized;

CString Inspection::FullVer;
CString Inspection::s_strClrModuleMd5Hash;
BOOL Inspection::GetDotNetVersionInfo(HMODULE g_hClrModule)
{
	WCHAR wszPath[MAX_PATH] = {0};
	::GetModuleFileNameW( g_hClrModule, wszPath, MAX_PATH);
	//VS_FIXEDFILEINFO *tVerInfo = (VS_FIXEDFILEINFO*)this;
	VS_FIXEDFILEINFO tVerInfo = {0};
	if ( CUtility::GetFileVersion( wszPath, &tVerInfo) && tVerInfo.dwSignature == 0xfeef04bd)
	{
		int nMajor = HIWORD(tVerInfo.dwFileVersionMS);		
		int nMinor = LOWORD(tVerInfo.dwFileVersionMS);		
		int nBuildMajor = HIWORD(tVerInfo.dwFileVersionLS);
		int nBuildMinor = LOWORD(tVerInfo.dwFileVersionLS);
		FullVer.Format(L"%d_%d_%d_%d",nMajor,nMinor,nBuildMajor,nBuildMinor);	
		if( nMajor < 4)
			g_tDotNetVersion = DotNetVersion_20;	
		else if ( nMajor == 4 && nMinor == 0 && nBuildMinor < 17001 )			
			g_tDotNetVersion = DotNetVersion_40;			
		else if (nMajor == 4 && nMinor < 6)
			g_tDotNetVersion = DotNetVersion_45;		
		else if (nMajor == 4 && nMinor == 6)// && nBuildMajor == 1038)	
			g_tDotNetVersion = DotNetVersion_46;
		else
			return FALSE;
	}
	return TRUE;	
}
