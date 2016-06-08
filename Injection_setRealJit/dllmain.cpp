// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Injection.h"
#include "MethodDesc.h"
#ifdef STATIC_LINK_EASYHOOK
BOOL APIENTRY EasyHookDllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	);
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{

#ifdef STATIC_LINK_EASYHOOK
	EasyHookDllMain( hModule, ul_reason_for_call, lpReserved);
#endif

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			::CreateThread( NULL
				, 0
				, CInjection::Initialize
				, hModule
				, 0
				, NULL
				);
			break;
		}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		CInjection::Uninitialize();
		break;
	}
	return TRUE;
}

