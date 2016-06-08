#pragma once
#include "MethodDesc.h"


////////////////////////////////////////////////////////////////////
/// LoadedMethodDescIterator
enum AssemblyIterationMode
{
	// Default, used by debugger's breakpoint controller.  Iterates through all
	// Assemblies associated with the specified AppDomain
	kModeAllADAssemblies,
	// Iterate through only the *unshared* assemblies associated with the specified
	// AppDomain.
	kModeUnsharedADAssemblies,
	// Rather than iterating through Assemblies associated with an AppDomain, just
	// iterate over all Assemblies associated with the SharedDomain
	kModeSharedDomainAssemblies,
};
enum AssemblyIterationFlags
{
	// load status flags
	kIncludeLoaded        = 0x00000001, // include assemblies that are already loaded
	// (m_level >= code:FILE_LOAD_DELIVER_EVENTS)
	kIncludeLoading       = 0x00000002, // include assemblies that are still in the process of loading
	// (all m_level values)
	kIncludeAvailableToProfilers
	= 0x00000020, // include assemblies available to profilers
	// See comment at code:DomainFile::IsAvailableToProfilers
	// Execution / introspection flags
	kIncludeExecution     = 0x00000004, // include assemblies that are loaded for execution only
	kIncludeIntrospection = 0x00000008, // include assemblies that are loaded for introspection only
	kIncludeFailedToLoad  = 0x00000010, // include assemblies that failed to load 
	// Collectible assemblies flags
	kExcludeCollectible   = 0x00000040, // Exclude all collectible assemblies
	kIncludeCollected     = 0x00000080, 
	// Include assemblies which were collected and cannot be referenced anymore. Such assemblies are not 
	// AddRef-ed. Any manipulation with them should be protected by code:GetAssemblyListLock.
	// Should be used only by code:LoaderAllocator::GCLoaderAllocators.

};  // enum AssemblyIterationFlags
enum ModuleIterationOption
{
	// include only modules that are already loaded (m_level >= FILE_LOAD_DELIVER_EVENTS)
	kModIterIncludeLoaded                = 1,
	// include all modules, even those that are still in the process of loading (all m_level values)
	kModIterIncludeLoading               = 2,
	// include only modules loaded just enough that profilers are notified of them.
	// (m_level >= FILE_LOAD_LOADLIBRARY).  See comment at code:DomainFile::IsAvailableToProfilers
	kModIterIncludeAvailableToProfilers  = 3,
};

class LoadedMethodDescIterator
{
	friend class CSymbolAddressCache;

	typedef void (LoadedMethodDescIterator::*PFN_LoadedMethodDescIteratorConstructor)(AppDomain * pAppDomain, Module *pModule,	mdMethodDef md);
	typedef void (LoadedMethodDescIterator::*PFN_LoadedMethodDescIteratorConstructor_v45)(AppDomain * pAppDomain, Module *pModule,	mdMethodDef md, AssemblyIterationMode mode);
	typedef void (LoadedMethodDescIterator::*PFN_LoadedMethodDescIteratorConstructor_v46)(AppDomain * pAppDomain, Module *pModule,	mdMethodDef md, AssemblyIterationMode mode, AssemblyIterationFlags a, ModuleIterationOption b);
	typedef void (LoadedMethodDescIterator::*PFN_Start)(AppDomain * pAppDomain, Module *pModule, mdMethodDef md);
	typedef BOOL ( LoadedMethodDescIterator::*PFN_Next_v4)(LPVOID pParam);
	typedef BOOL (LoadedMethodDescIterator::*PFN_Next_v2)(void);
	typedef MethodDesc* (LoadedMethodDescIterator::*PFN_Current)(void);
public:
	LoadedMethodDescIterator(AppDomain * pAppDomain, Module *pModule, mdMethodDef md)
	{
		memset( dummy, 0, sizeof(dummy));
		memset( dummy2, 0, sizeof(dummy2));
		if( s_pfnConstructor )
			(this->*s_pfnConstructor)( pAppDomain, pModule, md);
		if( s_pfnConstructor_v45 )
			(this->*s_pfnConstructor_v45)( pAppDomain, pModule, md, kModeAllADAssemblies);
		if( s_pfnConstructor_v46 )
			(this->*s_pfnConstructor_v46)( pAppDomain, pModule, md, kModeAllADAssemblies, (AssemblyIterationFlags)(kIncludeLoaded | kIncludeExecution), kModIterIncludeLoaded);
	}

	void Start(AppDomain * pAppDomain, Module *pModule, mdMethodDef md) 
	{ 
		(this->*s_pfnStart)( pAppDomain, pModule, md); 
	}

	BOOL Next() 
	{
		if( s_pfnNext_v4 )
			return (this->*s_pfnNext_v4)(dummy2); 

		if( s_pfnNext_v2 )
			return (this->*s_pfnNext_v2)(); 

		return FALSE;
	}
	MethodDesc* Current() { return (this->*s_pfnCurrent)(); }
private:
	// we don't know the exact size of LoadedMethodDescIterator, so add enough memory here
	BYTE dummy[10240]; 
	// class CollectibleAssemblyHolder<class DomainAssembly *> parameter for Next() in .Net4.0 and above	
	BYTE dummy2[10240]; 
	// constructor for .Net2.0 & .Net 4.0
	static PFN_LoadedMethodDescIteratorConstructor s_pfnConstructor;
	// constructor for .Net4.5
	static PFN_LoadedMethodDescIteratorConstructor_v45 s_pfnConstructor_v45;
	// constructor for .Net4.6
	static PFN_LoadedMethodDescIteratorConstructor_v46 s_pfnConstructor_v46;
	static PFN_Start s_pfnStart;
	static PFN_Next_v4 s_pfnNext_v4;
	static PFN_Next_v2 s_pfnNext_v2; 
	static PFN_Current s_pfnCurrent;

public:
	static void MatchAddress(PSYMBOL_INFOW pSymbolInfo)
	{
		LPVOID* pDest = NULL;
		if( wcscmp( L"LoadedMethodDescIterator::LoadedMethodDescIterator", pSymbolInfo->Name) == 0 )
		{
			switch(Inspection::g_tDotNetVersion)
			{
			case DotNetVersion_20:
			case DotNetVersion_40:
				pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnConstructor);
				break;			
			case DotNetVersion_45:
				pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnConstructor_v45);
			 if (LoadedMethodDescIterator::s_pfnConstructor_v45!=0)
			 {
			 return;
			 }
				break;
			case DotNetVersion_46:
				pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnConstructor_v46);
				break;

			default:
				ATLASSERT(FALSE);
				return;
			}
		}
		else if( wcscmp( L"LoadedMethodDescIterator::Next", pSymbolInfo->Name) == 0 )
		{
			switch(Inspection::g_tDotNetVersion)
			{
			case DotNetVersion_20:
				pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnNext_v2);
				break;

			case DotNetVersion_40:
			case DotNetVersion_45:
			case DotNetVersion_46:
				pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnNext_v4);
				break;

			default:
				ATLASSERT(FALSE);
				return;
			}
		}
		else if( wcscmp( L"LoadedMethodDescIterator::Start", pSymbolInfo->Name) == 0 )
			pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnStart);
		else if( wcscmp( L"LoadedMethodDescIterator::Current", pSymbolInfo->Name) == 0 )
			pDest = (LPVOID*)&(LoadedMethodDescIterator::s_pfnCurrent);
		
		if( pDest )
			*pDest = (LPVOID)pSymbolInfo->Address;
	}

	static BOOL IsInitialized(void)
	{
		return (s_pfnConstructor || s_pfnConstructor_v45 || s_pfnConstructor_v46) &&
			(s_pfnNext_v4 || s_pfnNext_v2) &&
			s_pfnCurrent;
	}
};

