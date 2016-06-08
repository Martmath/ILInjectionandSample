#pragma once
#include <map>
#include "MethodDesc.h"
#include "ICorJitCompiler.h"
// structure to store the IL code for replacement

class CInjection
{	
public:	
	static DWORD __stdcall Initialize(LPVOID lpParameter);
	static void Uninitialize(void);	
	
private:
	static BOOL SearchMethodAddresses(LPCWSTR lpwszSearchPath, BOOL bDisableSymSrv = TRUE);
};

