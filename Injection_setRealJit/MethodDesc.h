#pragma once
#include "PdbHelper.h"
#include "Utility.h"
#include "Inspection.h"
#include <map>

typedef ULONG32 mdToken;                // Generic token
typedef mdToken mdMethodDef;            // Method in this scope


enum CorJitResult
{
	// Note that I dont use FACILITY_NULL for the facility number,
	// we may want to get a 'real' facility number
	CORJIT_OK            =     NO_ERROR,
	CORJIT_BADCODE       =     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_NULL, 1),
	CORJIT_OUTOFMEM      =     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_NULL, 2),
	CORJIT_INTERNALERROR =     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_NULL, 3),
	CORJIT_SKIPPED       =     MAKE_HRESULT(SEVERITY_ERROR,FACILITY_NULL, 4),
};
class MethodDesc
{
	friend class CSymbolAddressCache;

	typedef void (MethodDesc::*PFN_Reset)(void);
	typedef BOOL (MethodDesc::*PFN_IsGenericMethodDefinition)(void);
	typedef ULONG (MethodDesc::*PFN_GetNumGenericMethodArgs)(void);
	typedef MethodDesc * (MethodDesc::*PFN_StripMethodInstantiation)(void);
	typedef BOOL (MethodDesc::*PFN_HasClassOrMethodInstantiation)(void);
	typedef BOOL (MethodDesc::*PFN_ContainsGenericVariables)(void);	
	typedef MethodDesc * (MethodDesc::*PFN_GetWrappedMethodDesc)(void);
	typedef AppDomain * (MethodDesc::*PFN_GetDomain)(void);
	typedef Module * (MethodDesc::*PFN_GetLoaderModule)(void);
public:
	void Reset(void) { (this->*s_pfnReset)(); }
	BOOL IsGenericMethodDefinition(void) { return (this->*s_pfnIsGenericMethodDefinition)(); }
	ULONG GetNumGenericMethodArgs(void) { return (this->*s_pfnGetNumGenericMethodArgs)(); }
	MethodDesc * StripMethodInstantiation(void) { return (this->*s_pfnStripMethodInstantiation)(); }
	BOOL HasClassOrMethodInstantiation(void)  { return (this->*s_pfnHasClassOrMethodInstantiation)(); }
	BOOL ContainsGenericVariables(void) { return (this->*s_pfnContainsGenericVariables)(); }
	MethodDesc * GetWrappedMethodDesc(void) { return (this->*s_pfnGetWrappedMethodDesc)(); }
	AppDomain * GetDomain(void) { return (this->*s_pfnGetDomain)(); }
	Module * GetLoaderModule(void) { return (this->*s_pfnGetLoaderModule)(); }	
//private:
	static PFN_Reset s_pfnReset;
	static PFN_IsGenericMethodDefinition s_pfnIsGenericMethodDefinition;
	static PFN_GetNumGenericMethodArgs s_pfnGetNumGenericMethodArgs;
	static PFN_StripMethodInstantiation s_pfnStripMethodInstantiation;
	static PFN_HasClassOrMethodInstantiation s_pfnHasClassOrMethodInstantiation;
	static PFN_ContainsGenericVariables s_pfnContainsGenericVariables;
	static PFN_GetWrappedMethodDesc s_pfnGetWrappedMethodDesc;
	static PFN_GetDomain s_pfnGetDomain;
	static PFN_GetLoaderModule s_pfnGetLoaderModule;

public:
	static void MatchAddress(LPCWSTR wszName, ULONG64 ulAddr)
	{
		LPVOID* pDest = NULL;
		if( wcscmp( L"MethodDesc::Reset", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnReset);
		else if( wcscmp( L"MethodDesc::IsGenericMethodDefinition", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnIsGenericMethodDefinition);
		else if( wcscmp( L"MethodDesc::GetNumGenericMethodArgs", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnGetNumGenericMethodArgs);
		else if( wcscmp( L"MethodDesc::StripMethodInstantiation", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnStripMethodInstantiation);
		else if( wcscmp( L"MethodDesc::HasClassOrMethodInstantiation", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnHasClassOrMethodInstantiation);
		else if( wcscmp( L"MethodDesc::ContainsGenericVariables", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnContainsGenericVariables);
		else if( wcscmp( L"MethodDesc::GetWrappedMethodDesc", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnGetWrappedMethodDesc);
		else if( wcscmp( L"MethodDesc::GetDomain", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnGetDomain);
		else if( wcscmp( L"MethodDesc::GetLoaderModule", wszName) == 0 )
			pDest = (LPVOID*)&(MethodDesc::s_pfnGetLoaderModule);

		if( pDest )
			*pDest = (LPVOID)ulAddr;
	}

	static BOOL IsInitialized(void)
	{
		return s_pfnReset &&
			s_pfnIsGenericMethodDefinition &&
			s_pfnGetNumGenericMethodArgs &&
			s_pfnStripMethodInstantiation &&
			s_pfnHasClassOrMethodInstantiation &&
			s_pfnHasClassOrMethodInstantiation &&
			s_pfnContainsGenericVariables &&
			s_pfnGetWrappedMethodDesc &&
			s_pfnGetDomain &&
			s_pfnGetLoaderModule;
	}
};
