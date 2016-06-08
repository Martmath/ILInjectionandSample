#pragma once

typedef BOOL (__stdcall * PFN_SymEnumTypesW)( HANDLE hProcess, ULONG64 BaseOfDll, PSYM_ENUMERATESYMBOLS_CALLBACKW EnumSymbolsCallback, PVOID UserContext);
typedef BOOL (__stdcall * PFN_SymGetTypeInfo)( HANDLE hProcess, DWORD64 ModBase, ULONG TypeId, IMAGEHLP_SYMBOL_TYPE_INFO GetType, PVOID pInfo);
typedef DWORD (__stdcall * PFN_SymGetOptions)(VOID);
typedef DWORD (__stdcall * PFN_SymSetOptions)(DWORD SymOptions);
typedef BOOL (__stdcall * PFN_SymInitializeW)(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess);
typedef DWORD64 (__stdcall * PFN_SymLoadModuleExW)(HANDLE hProcess,	HANDLE hFile, PCWSTR ImageName,	PCWSTR ModuleName, DWORD64 BaseOfDll, DWORD DllSize, PMODLOAD_DATA Data, DWORD Flags);
typedef BOOL (__stdcall * PFN_SymUnloadModule64)( HANDLE hProcess, DWORD64 BaseOfDll);
typedef BOOL (__stdcall * PFN_SymFromNameW)( HANDLE hProcess, PCWSTR Name, PSYMBOL_INFO Symbol);
typedef BOOL (__stdcall * PFN_SymEnumSymbolsW)( HANDLE hProcess, ULONG64 BaseOfDll, PCWSTR Mask, PSYM_ENUMERATESYMBOLS_CALLBACKW EnumSymbolsCallback, PVOID UserContext );
typedef BOOL (__stdcall * PFN_SymCleanup)(HANDLE hProcess);
typedef BOOL (__stdcall * PFN_SymSrvGetFileIndexInfoW)( PCWSTR File, PSYMSRV_INDEX_INFOW Info, DWORD Flags);


class CPdbHelper
{
public:
	CPdbHelper(LPCWSTR lpwszSearchPath, BOOL bDisableSymSrv = TRUE);
	~CPdbHelper();
	BOOL EnumClrSymbols(void);

	static BOOL Initialize(HMODULE hModule);

	static CString GetImageID(HMODULE hModule);

private:
	HANDLE m_hProcess;
	DWORD64 m_dwClrBaseAddr;
	static PFN_SymGetOptions s_pfnSymGetOptions ;
	static PFN_SymSetOptions s_pfnSymSetOptions;
	static PFN_SymInitializeW s_pfnSymInitializeW;
	static PFN_SymLoadModuleExW s_pfnSymLoadModuleExW;
	static PFN_SymUnloadModule64 s_pfnSymUnloadModule64;
	static PFN_SymFromNameW s_pfnSymFromNameW;
	static PFN_SymEnumSymbolsW s_pfnSymEnumSymbolsW;
	static PFN_SymCleanup s_pfnSymCleanup;
	static PFN_SymGetTypeInfo s_pfnSymGetTypeInfo;
	static PFN_SymEnumTypesW s_pfnSymEnumTypesW;
	static PFN_SymSrvGetFileIndexInfoW s_pfnSymSrvGetFileIndexInfoW;
	static BOOL CALLBACK EnumSymProc( PSYMBOL_INFOW pSymInfo, ULONG SymbolSize, PVOID UserContext);
};

