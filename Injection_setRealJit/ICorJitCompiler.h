#pragma once
#include "MethodDesc.h"
#include "LoadedMethodDescIterator.h"
#include "Injection.h"
#include <map>

typedef struct CORINFO_METHOD_STRUCT_*      CORINFO_METHOD_HANDLE;
typedef struct CORINFO_MODULE_STRUCT_*      CORINFO_MODULE_HANDLE;

enum CorInfoOptions
{
	CORINFO_OPT_INIT_LOCALS                 = 0x00000010, // zero initialize all variables
	CORINFO_GENERICS_CTXT_FROM_THIS         = 0x00000020, // is this shared generic code that access the generic context from the this pointer?  If so, then if the method has SEH then the 'this' pointer must always be reported and kept alive.
	CORINFO_GENERICS_CTXT_FROM_PARAMTYPEARG = 0x00000040, // is this shared generic code that access the generic context from the ParamTypeArg?  If so, then if the method has SEH then the 'ParamTypeArg' must always be reported and kept alive. Same as CORINFO_CALLCONV_PARAMTYPE
	CORINFO_GENERICS_CTXT_MASK              = (CORINFO_GENERICS_CTXT_FROM_THIS |
	CORINFO_GENERICS_CTXT_FROM_PARAMTYPEARG),
	CORINFO_GENERICS_CTXT_KEEP_ALIVE        = 0x00000080, // Keep the generics context alive throughout the method even if there is no explicit use, and report its location to the CLR
};

struct CORINFO_METHOD_INFO
{
	CORINFO_METHOD_HANDLE       ftn;
	CORINFO_MODULE_HANDLE       scope;
	BYTE *                      ILCode;
	unsigned                    ILCodeSize;
	unsigned short              maxStack;
	unsigned short              EHcount;
	CorInfoOptions              options;
};

typedef struct _ILCodeBuffer
{
	LPBYTE						pBuffer;
	DWORD						dwSize;
	BOOL						bIsGeneric;
} ILCodeBuffer, *LPILCodeBuffer;

class ICorJitCompiler
{
	friend class CInjection;	
	friend class CSymbolAddressCache;
	friend class Jit;
public:
	static ICorJitCompiler * Instance;
	static Jit * newJit;

	typedef int (__stdcall ICorJitCompiler::*PFN_compileMethod)(ICorJitInfo * pJitInfo, CORINFO_METHOD_INFO * pMethodInfo,
		UINT nFlags, LPBYTE * pEntryAddress, ULONG * pSizeOfCode);
	typedef void (ICorJitCompiler::*PFN_clearCache)(void);
	typedef BOOL (ICorJitCompiler::*PFN_isCacheCleanupRequired)(); 
	typedef void (ICorJitCompiler::*PFN_ProcessShutdownWork)(ULONG_PTR  info);
	typedef void (ICorJitCompiler::*PFN_getVersionIdentifier)(GUID* versionIdentifier);
	typedef  unsigned (ICorJitCompiler::*PFN_getMaxIntrinsicSIMDVectorLength)( DWORD cpuCompileFlags);
	typedef void (ICorJitCompiler::*PFN_setRealJit)(Jit* realJitCompiler);	

	PFN_compileMethod ñompileMethodintercept;
	PFN_clearCache clearCacheintercept;
	PFN_isCacheCleanupRequired isCacheCleanupRequiredintercept;
	PFN_ProcessShutdownWork ProcessShutdownWorkintercept;
	PFN_getVersionIdentifier getVersionIdentifierintercept;
	PFN_getMaxIntrinsicSIMDVectorLength	getMaxIntrinsicSIMDVectorLengthintercept;
	PFN_setRealJit	setRealJitintercept;
	
	static std::map< CORINFO_METHOD_HANDLE, ILCodeBuffer> s_mpILBuffers;		

	static BOOL StartUpdateILCodes( MethodTable * pMethodTable, CORINFO_METHOD_HANDLE pMethodHandle,
		mdMethodDef md, LPBYTE pBuffer, DWORD dwSize);
private:
	int __stdcall compileMethod(ICorJitInfo * pJitInfo,CORINFO_METHOD_INFO * pCorMethodInfo,
		UINT nFlags, LPBYTE * pEntryAddress, ULONG * pSizeOfCode);	

	void clearCache()
	{
		(Instance->*setRealJitintercept)(nullptr);
		(Instance->*clearCacheintercept)();
		(Instance->*setRealJitintercept)(newJit);
	};

	BOOL isCacheCleanupRequired() 
	{
		(Instance->*setRealJitintercept)(nullptr);	
		BOOL r= (Instance->*isCacheCleanupRequiredintercept)();
		(Instance->*setRealJitintercept)(newJit);	
		return r;
	};

   void ProcessShutdownWork(ULONG_PTR  info) 
   {
	   (Instance->*setRealJitintercept)(nullptr);	
	   (Instance->*ProcessShutdownWorkintercept)(info);
	   (Instance->*setRealJitintercept)(newJit);			
   };
   void getVersionIdentifier(  GUID*   versionIdentifier )
   {
	   (Instance->*setRealJitintercept)(nullptr);
	   (Instance->*getVersionIdentifierintercept)(versionIdentifier);
	   (Instance->*setRealJitintercept)(newJit);
   };
   unsigned getMaxIntrinsicSIMDVectorLength(DWORD cpuCompileFlags)
   {
	   (Instance->*setRealJitintercept)(nullptr);
	   unsigned r = (Instance->*getMaxIntrinsicSIMDVectorLengthintercept)(cpuCompileFlags);
	   (Instance->*setRealJitintercept)(newJit);
	   return r;
   };
   void setRealJit() 
   { 
	   (Instance->*setRealJitintercept)( newJit);
   };

	ICorJitCompiler() ;  
    ~ICorJitCompiler();
	ICorJitCompiler(ICorJitCompiler const&); 
    ICorJitCompiler& operator= (ICorJitCompiler const&); 

};
class Jit
{
public:
	virtual int __stdcall compileMethod(ICorJitInfo * pJitInfo,CORINFO_METHOD_INFO * pCorMethodInfo,
		 UINT nFlags, LPBYTE * pEntryAddress, ULONG * pSizeOfCode)
	{
		return ICorJitCompiler::Instance->compileMethod(pJitInfo, pCorMethodInfo, nFlags,
		 pEntryAddress,  pSizeOfCode);
	};

	virtual void clearCache()
	{
		ICorJitCompiler::Instance->clearCache();
	};	

	virtual BOOL isCacheCleanupRequired() 
	{
		return ICorJitCompiler::Instance->isCacheCleanupRequired();		
	};

	virtual void ProcessShutdownWork(ULONG_PTR  info) 
	{
	   ICorJitCompiler::Instance->ProcessShutdownWork(info);			
	};

	virtual void getVersionIdentifier(  GUID*   versionIdentifier )
	{
		ICorJitCompiler::Instance->getVersionIdentifier(versionIdentifier);
	}; 

	virtual unsigned getMaxIntrinsicSIMDVectorLength(DWORD cpuCompileFlags) 
	{
		return  ICorJitCompiler::Instance->getMaxIntrinsicSIMDVectorLength(cpuCompileFlags);
	};
	
	virtual void setRealJit() 
	{ 
		ICorJitCompiler::Instance->setRealJit();
	};	
};

