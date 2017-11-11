#include "stdafx.h"
#include "ICorJitCompiler.h"


//----------Exported-------------
extern "C"
BOOL __declspec (dllexport) __stdcall 
UpdateILCodes( MethodTable * pMethodTable, CORINFO_METHOD_HANDLE pMethodHandle, mdMethodDef md, LPBYTE pBuffer, DWORD dwSize)
{
	return ICorJitCompiler::StartUpdateILCodes(pMethodTable, pMethodHandle, md, pBuffer, dwSize);
}

// enum{ Status_Uninitialized = 0, Status_Ready = 1, Status_Error = -1 };
extern "C"
	INT __declspec (dllexport) __stdcall 
	GetStatus()
{
	return Inspection::GetStatus();
}

// wait until the initialization is completed
extern "C"
	INT __declspec (dllexport) __stdcall 
	WaitForIntializationCompletion()
{
	::WaitForSingleObject( Inspection::s_hEvent, INFINITE);
	return Inspection::GetStatus();
}
//----------Exported-------------

ICorJitCompiler::PFN_compileMethod  compileMethodcache = NULL;
ICorJitCompiler * ICorJitCompiler::Instance = NULL;
std::map< CORINFO_METHOD_HANDLE, ILCodeBuffer> ICorJitCompiler::s_mpILBuffers;

int __stdcall ICorJitCompiler::compileMethod(ICorJitInfo * pJitInfo, CORINFO_METHOD_INFO * pCorMethodInfo,
											 UINT nFlags, LPBYTE * pEntryAddress, ULONG * pSizeOfCode)
{	
	LPBYTE pOriginalILCode = pCorMethodInfo->ILCode;
	unsigned int nOriginalSize = pCorMethodInfo->ILCodeSize;
	// find the method to be replaced
	ILCodeBuffer tILCodeBuffer = {0};
	if( pCorMethodInfo && Inspection::GetStatus() == 1 )
	{
		MethodDesc * pMethodDesc = (MethodDesc*)pCorMethodInfo->ftn;
		LPCUTF8 test = pMethodDesc->GetMethodName();
		// if the current method is not found, try to search its generic definition method*/
		std::map< CORINFO_METHOD_HANDLE, ILCodeBuffer>::iterator iter = s_mpILBuffers.find((CORINFO_METHOD_HANDLE)pMethodDesc);
		if( iter == s_mpILBuffers.end() && pMethodDesc->HasClassOrMethodInstantiation() )
		{
			MethodDesc * pStripMD = pMethodDesc->StripMethodInstantiation();		
			if( pStripMD )
				iter = s_mpILBuffers.find((CORINFO_METHOD_HANDLE)pStripMD);

			if( iter == s_mpILBuffers.end() )
			{
				MethodDesc * pWrappedMD = pMethodDesc->GetWrappedMethodDesc();
				if( pWrappedMD )
					iter = s_mpILBuffers.find((CORINFO_METHOD_HANDLE)pWrappedMD);
			}
		}
		if( iter != s_mpILBuffers.end() )
		{
			tILCodeBuffer = iter->second;
			pCorMethodInfo->ILCode = tILCodeBuffer.pBuffer;
			pCorMethodInfo->ILCodeSize = tILCodeBuffer.dwSize;
			if( !tILCodeBuffer.bIsGeneric )
					s_mpILBuffers.erase(iter);
		}
	}
	
	CorJitResult result = 	(CorJitResult) (this->*compileMethodcache)( pJitInfo, pCorMethodInfo, nFlags, pEntryAddress, pSizeOfCode);	

	if( tILCodeBuffer.pBuffer )
	{
		pCorMethodInfo->ILCode = pOriginalILCode;
		pCorMethodInfo->ILCodeSize = nOriginalSize;
		if( !tILCodeBuffer.bIsGeneric )
			LocalFree(tILCodeBuffer.pBuffer);		
	}

	return result;
}

BOOL ICorJitCompiler::StartUpdateILCodes( MethodTable * pMethodTable, CORINFO_METHOD_HANDLE pMethodHandle,
										 mdMethodDef md, LPBYTE pBuffer, DWORD dwSize)
{
	if( Inspection::s_nStatus != Inspection::Status_Ready || !pMethodHandle )
		return FALSE;

	MethodDesc * pMethodDesc = (MethodDesc*)pMethodHandle;
	pMethodDesc->Reset();

	MethodDesc * pStripMethodDesc = pMethodDesc->StripMethodInstantiation();
	if( pStripMethodDesc )
		pStripMethodDesc->Reset();

	ILCodeBuffer tILCodeBuffer;
	tILCodeBuffer.pBuffer = pBuffer;
	tILCodeBuffer.dwSize = dwSize;
	tILCodeBuffer.bIsGeneric = FALSE;

	// this is a generic method
	if( pMethodDesc->ContainsGenericVariables() || pMethodDesc->HasClassOrMethodInstantiation() )
	{
		tILCodeBuffer.bIsGeneric = TRUE;
		MethodDesc * pWrappedMethodDesc = pMethodDesc->GetWrappedMethodDesc();
		if( pWrappedMethodDesc )
		{
			pWrappedMethodDesc->Reset();
		}

		// find out all the instantiations of this generic method
		Module * pModule = pMethodDesc->GetLoaderModule();
		AppDomain * pAppDomain = pMethodDesc->GetDomain();
		if( pModule )
		{
			LoadedMethodDescIterator * pLoadedMethodDescIter = new LoadedMethodDescIterator( pAppDomain, pModule, md);
			while(pLoadedMethodDescIter->Next())
			{
				MethodDesc * pMD = pLoadedMethodDescIter->Current();
				if( pMD )
					pMD->Reset();
			}
			delete pLoadedMethodDescIter;
		}
	}

	std::map< CORINFO_METHOD_HANDLE, ILCodeBuffer>::iterator iter = s_mpILBuffers.find(pMethodHandle);
	if( iter != s_mpILBuffers.end() )
	{
		LocalFree(iter->second.pBuffer);
		s_mpILBuffers.erase(iter);
	}
	
	s_mpILBuffers.insert( std::pair< CORINFO_METHOD_HANDLE, ILCodeBuffer>( pMethodHandle, tILCodeBuffer) );

	return TRUE;
}
