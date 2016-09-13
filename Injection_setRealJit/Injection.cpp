#include "StdAfx.h"
#include "Injection.h"
#include "PdbHelper.h"
#include "MethodDesc.h"
#include "LoadedMethodDescIterator.h"
#include "SymbolAddressCache.h"
#include "Utility.h"
#include "Inspection.h"

#include "cor.h"
#include <stddef.h>


#if defined(_WIN64)
#define DBG_HELP_DLL _T("x64\\dbg64.dll")
#else
#define DBG_HELP_DLL _T("x86\\dbg32.dll")
#endif

#if defined(_WIN64)
#define SYMCHK_EXE _T("x64\\symchk.exe")
#else
#define SYMCHK_EXE _T("x86\\symchk.exe")
#endif

ULONG_PTR *(__stdcall *p_getJit)();

DWORD __stdcall CInjection::Initialize(LPVOID lpParameter )
{
	USES_CONVERSION;	
	// find the JIT module
	g_hJitModule = GetModuleHandleA("clrjit.dll");
	if( !g_hJitModule )
		g_hJitModule = GetModuleHandleA("mscorjit.dll");
	if( g_hJitModule == NULL )
	{
		Inspection::s_nStatus = Inspection::Status_Error_JITNotFound;
		SetEvent( Inspection::s_hEvent );
		return FALSE;
	}
	// find the CLR module
	g_hClrModule = GetModuleHandleA("clr.dll");
	if( g_hClrModule )
	{	
		if(!Inspection::GetDotNetVersionInfo(g_hClrModule) )		
			Inspection::s_nStatus = Inspection::Status_Error_CLRNotFound;		
	}
	else
	{
		g_hClrModule = GetModuleHandleA("mscorwks.dll");
		if(!(g_hClrModule && Inspection::GetDotNetVersionInfo(g_hClrModule))) 		
			Inspection::s_nStatus = Inspection::Status_Error_CLRNotFound;	
	}
	if (Inspection::s_nStatus == Inspection::Status_Error_CLRNotFound)
	{
		SetEvent( Inspection::s_hEvent );
		return FALSE;
	}
	// try to quick load the symbol address base on the binary hash
	if( !CSymbolAddressCache::TryCache() )
	{		
		HMODULE hInstance = (HMODULE)lpParameter;
		// get the current directory
		CString strCurrentDir = CUtility::GetCurrentDirectory();

		// dbghelp.dll
		{
			CString strDbgHelpDll;
			strDbgHelpDll.Format(_T("%s%s"), strCurrentDir, DBG_HELP_DLL);
			HMODULE hModule = ::LoadLibrary(strDbgHelpDll);
			if (hModule == NULL || !CPdbHelper::Initialize(hModule))
			{
				Inspection::s_nStatus = Inspection::Status_Error_DbgHelpNotFound;
				SetEvent(Inspection::s_hEvent);
				return FALSE;
			}
		}
		
		
		
		// get the pdb directory
		CString strDestPath(strCurrentDir);
		{
			strDestPath.AppendFormat( _T("PDB_symbols\\") );
			::CreateDirectory(strDestPath, NULL);
		}
		
		// copy the CLR dll
		{
			TCHAR tszFilename[MAX_PATH] = {0};
			GetModuleFileName( g_hClrModule, tszFilename, MAX_PATH);
			::CopyFile( tszFilename, strDestPath + CUtility::GetFileName(tszFilename), FALSE);
		}
		// Set Environment Variable 
		{
			CString strVariable;
			strVariable.Format( _T("symsrv*symsrv.dll*%s*http://msdl.microsoft.com/download/symbols"), strDestPath);
			SetEnvironmentVariable( _T("_NT_SYMBOL_PATH"), strVariable.GetBuffer());
			strVariable.ReleaseBuffer();
		}

		if( !SearchMethodAddresses(T2W(strDestPath.GetBuffer())) )
		{
			CString strCmd;
			strCmd.Format( _T("\"%s%s\" /if \"%s*.dll\" /s symsrv*symsrv.dll*%s*http://msdl.microsoft.com/download/symbols"),
				strCurrentDir, SYMCHK_EXE, strDestPath, strDestPath);
			ATLTRACE( _T("\n%s"), strCmd);

			STARTUPINFO si = { sizeof(si) };
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			PROCESS_INFORMATION  pi = {0};
			BOOL bRet = CreateProcess( NULL, strCmd.GetBuffer(), 
				NULL, NULL, FALSE, 0, NULL, strCurrentDir, &si, &pi);
			strCmd.ReleaseBuffer();
			if( !bRet )
			{
				Inspection::s_nStatus = Inspection::Status_Error_DownloadPDBFailed;
				SetEvent( Inspection::s_hEvent );
				return FALSE;
			}
			WaitForSingleObject(pi.hProcess, INFINITE); 

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			// find all the pdb files
			if( !SearchMethodAddresses(NULL) )
				SearchMethodAddresses(NULL, FALSE);
		}	
		// cache the address offset according to the binary hash		
		if( MethodDesc::IsInitialized() && LoadedMethodDescIterator::IsInitialized() )
		{
			CSymbolAddressCache::GenerateClrCache();
		}
	}
	
	p_getJit = (ULONG_PTR *(__stdcall *)()) GetProcAddress( g_hJitModule, "getJit");
	if (p_getJit)
	{
		ICorJitCompiler ::Instance = (ICorJitCompiler *) *((ULONG_PTR *) p_getJit());
		ICorJitCompiler ::newJit = new Jit();
		ICorJitCompiler ::Instance->setRealJit();	
	}

	Inspection::s_nStatus = Inspection::Status_Ready;
	SetEvent( Inspection::s_hEvent );
	return TRUE;
}

void CInjection::Uninitialize(void)
{
	if( Inspection::s_nStatus == Inspection::Status_Ready )
	{
		Inspection::s_nStatus = Inspection::Status_Uninitialized;		
	}
	
	std::map< CORINFO_METHOD_HANDLE, ILCodeBuffer>::iterator iter;
	for( iter = ICorJitCompiler::s_mpILBuffers.begin(); iter != ICorJitCompiler::s_mpILBuffers.end(); iter ++)
	{
		LocalFree(iter->second.pBuffer);
	}
	ICorJitCompiler::s_mpILBuffers.clear();

	if( Inspection::s_hEvent )
	{
		CloseHandle(Inspection::s_hEvent);
		Inspection::s_hEvent = NULL;
	}
}

BOOL CInjection::SearchMethodAddresses(LPCWSTR lpwszSearchPath, BOOL bDisableSymSrv /* = TRUE */)
{
	CPdbHelper tPdbHelper(lpwszSearchPath, bDisableSymSrv);		
	return tPdbHelper.EnumClrSymbols() && MethodDesc::IsInitialized() && LoadedMethodDescIterator::IsInitialized();
}





