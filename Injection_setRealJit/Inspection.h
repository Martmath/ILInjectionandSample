#pragma once
#if defined(_WIN64)
#define ENVIRONMENT _T("64")
#else
#define ENVIRONMENT _T("32")
#endif
class Module;
class AppDomain;
class ICorJitInfo{};
class MethodTable {};

//extern DotNetVersion g_tDotNetVersion;
extern HMODULE g_hJitModule;
extern HMODULE g_hClrModule;
enum DotNetVersion
{
	// .Net Framework 2.0/3.5
	DotNetVersion_20,
	// .Net Framework 4.0
	DotNetVersion_40,
	// .Net Framework 4.5
	DotNetVersion_45,
	// .Net Framework 4.6
	DotNetVersion_46,
};

struct Inspection //:VS_FIXEDFILEINFO
{
public:
enum
{
	Status_Ready = 1,
	Status_Uninitialized = 0,
	Status_Error_HookCompileMethodFailed = -1,
	Status_Error_LoadedMethodDescIteratorInitializationFailed = -2,
	Status_Error_MethodDescInitializationFailed = -3,
	Status_Error_DbgHelpNotFound = -4,
	Status_Error_JITNotFound = -5,
	Status_Error_DownloadPDBFailed = -6,
	Status_Error_CLRNotFound = -7,
};
	static CString FullVer;	
	static CString s_strClrModuleMd5Hash;
	static INT s_nStatus;
	static HANDLE s_hEvent;
	static DotNetVersion g_tDotNetVersion;
	static INT GetStatus(void) { return s_nStatus; }	
	static CString GetFileCacheName(void) { 
		return s_strClrModuleMd5Hash + L"#"+ENVIRONMENT+ L"_" + FullVer; }	
	static BOOL GetDotNetVersionInfo(HMODULE g_hClrModule);		
};

