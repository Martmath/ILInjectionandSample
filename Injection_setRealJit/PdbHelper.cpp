#include "StdAfx.h"
#include "PdbHelper.h"
#include "MethodDesc.h"
#include "LoadedMethodDescIterator.h"

// dbghelp.dll functions
PFN_SymGetOptions CPdbHelper::s_pfnSymGetOptions = NULL;
PFN_SymSetOptions CPdbHelper::s_pfnSymSetOptions = NULL;
PFN_SymInitializeW CPdbHelper::s_pfnSymInitializeW = NULL;
PFN_SymLoadModuleExW CPdbHelper::s_pfnSymLoadModuleExW = NULL;
PFN_SymUnloadModule64 CPdbHelper::s_pfnSymUnloadModule64 = NULL;
PFN_SymFromNameW CPdbHelper::s_pfnSymFromNameW = NULL;
PFN_SymEnumSymbolsW CPdbHelper::s_pfnSymEnumSymbolsW = NULL;
PFN_SymCleanup CPdbHelper::s_pfnSymCleanup = NULL;
PFN_SymGetTypeInfo CPdbHelper::s_pfnSymGetTypeInfo = NULL;
PFN_SymEnumTypesW CPdbHelper::s_pfnSymEnumTypesW = NULL;
PFN_SymSrvGetFileIndexInfoW CPdbHelper::s_pfnSymSrvGetFileIndexInfoW = NULL;

BOOL CPdbHelper::Initialize(HMODULE hModule)
	
{	
	s_pfnSymGetOptions = (PFN_SymGetOptions)::GetProcAddress( hModule, "SymGetOptions");
	s_pfnSymSetOptions = (PFN_SymSetOptions)::GetProcAddress( hModule, "SymSetOptions");
	s_pfnSymInitializeW = (PFN_SymInitializeW)::GetProcAddress( hModule, "SymInitializeW");
	s_pfnSymLoadModuleExW = (PFN_SymLoadModuleExW)::GetProcAddress( hModule, "SymLoadModuleExW");
	s_pfnSymUnloadModule64 = (PFN_SymUnloadModule64)::GetProcAddress( hModule, "SymUnloadModule64");
	s_pfnSymFromNameW = (PFN_SymFromNameW)::GetProcAddress( hModule, "SymFromNameW");
	s_pfnSymEnumSymbolsW = (PFN_SymEnumSymbolsW)::GetProcAddress( hModule, "SymEnumSymbolsW");
	s_pfnSymCleanup = (PFN_SymCleanup)::GetProcAddress( hModule, "SymCleanup");
	s_pfnSymGetTypeInfo = (PFN_SymGetTypeInfo)::GetProcAddress( hModule, "SymGetTypeInfo");
	s_pfnSymEnumTypesW = (PFN_SymEnumTypesW)::GetProcAddress( hModule, "SymEnumTypesW");
	s_pfnSymSrvGetFileIndexInfoW = (PFN_SymSrvGetFileIndexInfoW)::GetProcAddress( hModule, "SymSrvGetFileIndexInfoW");

	return s_pfnSymGetOptions && 
		s_pfnSymSetOptions &&
		s_pfnSymInitializeW &&
		s_pfnSymLoadModuleExW &&
		s_pfnSymUnloadModule64 &&
		s_pfnSymFromNameW &&
		s_pfnSymEnumSymbolsW &&
		s_pfnSymCleanup &&
		s_pfnSymGetTypeInfo &&
		s_pfnSymEnumTypesW &&
		s_pfnSymSrvGetFileIndexInfoW;
}

CPdbHelper::CPdbHelper(LPCWSTR lpwszSearchPath, BOOL bDisableSymSrv /* = TRUE */)
	: m_hProcess(NULL)
	, m_dwClrBaseAddr(0)
{
	ATLASSERT( s_pfnSymSetOptions && s_pfnSymGetOptions && s_pfnSymInitializeW );

	DWORD dwOptions = s_pfnSymGetOptions(); 
	if( bDisableSymSrv )
		dwOptions |= SYMOPT_DISABLE_SYMSRV_AUTODETECT;
	dwOptions |= SYMOPT_EXACT_SYMBOLS | SYMOPT_DEFERRED_LOADS;
	s_pfnSymSetOptions(dwOptions);

	m_hProcess = GetCurrentProcess();
	s_pfnSymInitializeW( m_hProcess, lpwszSearchPath, FALSE);
}

CPdbHelper::~CPdbHelper()
{
	ATLASSERT( s_pfnSymCleanup );
	s_pfnSymCleanup(m_hProcess);
}

BOOL CPdbHelper::EnumClrSymbols(void)
{
	ATLASSERT( s_pfnSymLoadModuleExW && s_pfnSymUnloadModule64);

	WCHAR wszPath[MAX_PATH] = {0};
	::GetModuleFileNameW( g_hClrModule, wszPath, MAX_PATH);
	m_dwClrBaseAddr = s_pfnSymLoadModuleExW( m_hProcess
		, NULL
		, wszPath
		, NULL
		, (DWORD64)g_hClrModule
		, 0
		, NULL
		, 0
		);
	if( m_dwClrBaseAddr == 0 )
	{
		if( GetLastError() != ERROR_SUCCESS )
			return FALSE;
		else
			m_dwClrBaseAddr = (DWORD64)g_hClrModule;
	}
	s_pfnSymEnumSymbolsW(m_hProcess, m_dwClrBaseAddr, NULL, CPdbHelper::EnumSymProc, this);
	s_pfnSymUnloadModule64( m_hProcess, (DWORD64)g_hClrModule);
	return TRUE;
}

BOOL CALLBACK CPdbHelper::EnumSymProc( PSYMBOL_INFOW pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	MethodDesc::MatchAddress( pSymInfo->Name, pSymInfo->Address);
	LoadedMethodDescIterator::MatchAddress(pSymInfo);

	return TRUE;
}




CString CPdbHelper::GetImageID(HMODULE hModule)
{
	WCHAR wszFileName[MAX_PATH] = {0};
	GetModuleFileNameW( hModule, wszFileName, MAX_PATH);

	SYMSRV_INDEX_INFO tIndexInfo = {0};
	tIndexInfo.sizeofstruct = sizeof(tIndexInfo);
	BOOL bRet = s_pfnSymSrvGetFileIndexInfoW( wszFileName, &tIndexInfo, 0);

	CString strGuid;
	if( bRet )
	{
		LPOLESTR pGuid;
		HRESULT hr = StringFromCLSID( tIndexInfo.guid, &pGuid);
		if( SUCCEEDED(hr) )
		{
			USES_CONVERSION;
			strGuid.Format( _T("%s"), OLE2T(pGuid));
			CoTaskMemFree(pGuid);

			strGuid.Replace( _T("{"), _T(""));
			strGuid.Replace( _T("}"), _T(""));
			strGuid.Replace( _T("-"), _T(""));
			strGuid.AppendFormat(_T("%X"), tIndexInfo.age);
		}
	}

	return strGuid;
}