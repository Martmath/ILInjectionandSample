#pragma once
#include "MethodDesc.h"
#include "LoadedMethodDescIterator.h"
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
public:

	static ICorJitCompiler * Instance;		
	typedef int (__stdcall ICorJitCompiler::*PFN_compileMethod)(ICorJitInfo * pJitInfo, CORINFO_METHOD_INFO * pMethodInfo,
		UINT nFlags, LPBYTE * pEntryAddress, ULONG * pSizeOfCode);
	PFN_compileMethod ñompileMethodintercept;

	static std::map< CORINFO_METHOD_HANDLE, ILCodeBuffer> s_mpILBuffers;	

	int __stdcall compileMethod(ICorJitInfo * pJitInfo,CORINFO_METHOD_INFO * pCorMethodInfo,
		UINT nFlags, LPBYTE * pEntryAddress, ULONG * pSizeOfCode);	
	static BOOL StartUpdateILCodes( MethodTable * pMethodTable, CORINFO_METHOD_HANDLE pMethodHandle,
		mdMethodDef md, LPBYTE pBuffer, DWORD dwSize);
private:
	ICorJitCompiler() ;  
    ~ICorJitCompiler();
	ICorJitCompiler(ICorJitCompiler const&); 
    ICorJitCompiler& operator= (ICorJitCompiler const&); 
};
extern ICorJitCompiler::PFN_compileMethod compileMethodcache;

