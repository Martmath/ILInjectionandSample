#include "StdAfx.h"
#include "SymbolAddressCache.h"
#include "MethodDesc.h"
#include "LoadedMethodDescIterator.h"
#include "Utility.h"
#include <atlfile.h>
#include "resource.h"

#define SAVE_OFFSET(POINTER) \
	if( POINTER ) {\
		pTemp = (LPVOID *)&(POINTER);\
		ulOffset = (ULONG64)(*pTemp) - ulBaseAddr;\
	}\
	else\
		ulOffset = 0;	\
	strContent.AppendFormat( _T("\n%llu;%s"), ulOffset, A2T(#POINTER));


#define LOAD_OFFSET(POINTER) \
	iter = mpOffset.find(A2T(#POINTER)); \
	pTemp = (LPVOID *)&POINTER; \
	if( iter != mpOffset.end() ) \
	{ \
		if( iter->second > 0 ) \
			*pTemp = (LPVOID)(iter->second + ulBaseAddr); \
		else \
			(*pTemp) = NULL; \
	}\
	else \
		(*pTemp) = NULL;


BOOL CSymbolAddressCache::TryCache(void)
{
	
	Inspection::s_strClrModuleMd5Hash = CUtility::ComputeMd5HashForModule(g_hClrModule);

	USES_CONVERSION;	
	std::map<CString, ULONG64>::iterator iter;
	LPVOID * pTemp;	

	ULONG64 ulBaseAddr = (ULONG64)g_hClrModule;
	std::map<CString, ULONG64> mpOffset = LoadCache(Inspection::GetFileCacheName());// Inspection::GetFileCacheName());

	LOAD_OFFSET(MethodDesc::s_pfnReset);
	LOAD_OFFSET(MethodDesc::s_pfnIsGenericMethodDefinition);
	LOAD_OFFSET(MethodDesc::s_pfnGetNumGenericMethodArgs);
	LOAD_OFFSET(MethodDesc::s_pfnStripMethodInstantiation);
	LOAD_OFFSET(MethodDesc::s_pfnHasClassOrMethodInstantiation);
	LOAD_OFFSET(MethodDesc::s_pfnContainsGenericVariables);
	LOAD_OFFSET(MethodDesc::s_pfnGetWrappedMethodDesc);
	LOAD_OFFSET(MethodDesc::s_pfnGetDomain);
	LOAD_OFFSET(MethodDesc::s_pfnGetLoaderModule);

	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnConstructor);
	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnConstructor_v45);
	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnConstructor_v46);
	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnStart);
	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnNext_v4);
	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnNext_v2);
	LOAD_OFFSET(LoadedMethodDescIterator::s_pfnCurrent);


	return MethodDesc::IsInitialized() && LoadedMethodDescIterator::IsInitialized();
}


BOOL CSymbolAddressCache::ParseContent(CString strContent, std::map<CString, ULONG64> & mpOffset)
{
	int nIndex = strContent.Find('\n');
	if( nIndex < 0 )
		nIndex = strContent.GetLength();
	while( !strContent.IsEmpty() && nIndex > 0 )
	{
		CString strItem = strContent.Mid( 0, nIndex);
		strContent = strContent.Mid( nIndex + 1, strContent.GetLength() - nIndex - 1);
		nIndex = strContent.Find('\n');
		if( nIndex < 0 )
			nIndex = strContent.GetLength();

		int nFind = strItem.Find(';');
		if( nFind <= 0 )
			continue;

		CString strPart1 = strItem.Mid( 0, nFind);
		CString strPart2 = strItem.Mid( nFind + 1, strItem.GetLength() - nFind - 1);
		ULONG64 ullOffset = _tstoi64( strPart1 );

		mpOffset[strPart2] = ullOffset;
	}
	return mpOffset.size() > 0;
}

std::map<CString, ULONG64> CSymbolAddressCache::LoadCache(LPCTSTR lpszMd5Hash)
{
	std::map<CString, ULONG64> mpOffset;
	if( TryLoadPredefinedCache( lpszMd5Hash, mpOffset) )
		return mpOffset;

	{
		USES_CONVERSION;
		CHAR szBuffer[10240] = {0};

		CString strFileName = CUtility::GetCurrentDirectory();
		strFileName.Append( _T("cache\\") );
		strFileName.AppendFormat( _T("%s.cache"), lpszMd5Hash);

		CAtlFile file;
		HRESULT hr = file.Create( strFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
		if( FAILED(hr) )
			return mpOffset;

		ULONGLONG ullSize = 0;
		file.GetSize(ullSize);
		ATLASSERT( ullSize < sizeof(szBuffer) );
		file.Read( szBuffer, sizeof(szBuffer) - 1);
		file.Close();

		CString strContent = A2T(szBuffer);
		ParseContent( strContent, mpOffset);
	}	

	return mpOffset;
}

BOOL CSymbolAddressCache::TryLoadPredefinedCache(LPCTSTR lpszMd5Hash, std::map<CString, ULONG64> & mpOffset)
{
	CHAR szTemp[4098] = {0};
	BOOL bRet = FALSE;

	HMODULE hModule = CUtility::GetCurrentModule();

	CString strResName;
	strResName.Format( _T("_%s"), lpszMd5Hash);
	
	HRSRC hRsrc = FindResource( hModule, strResName, _T("RT_RCDATA") );
	if( !hRsrc )
		hRsrc = FindResource( hModule, strResName, RT_RCDATA);
	if( !hRsrc )
		return FALSE;

	DWORD dwSize = SizeofResource(hModule, hRsrc);
	if (dwSize == 0 || dwSize > (sizeof(szTemp) - 1) )
		return FALSE;

	HGLOBAL hGlobal = LoadResource( hModule, hRsrc);
	if( !hGlobal )
		return FALSE;

	LPVOID pBuffer = LockResource(hGlobal);
	if( pBuffer )
	{
		USES_CONVERSION;
		memcpy_s( szTemp, sizeof(szTemp), pBuffer, dwSize);

		bRet = ParseContent( A2T(szTemp), mpOffset);
	}

	
	FreeResource(hGlobal);

	return bRet;
}



void CSymbolAddressCache::GenerateClrCache(void)
{
	USES_CONVERSION;
	ULONG64 ulBaseAddr = (ULONG64)g_hClrModule;
	ULONG64 ulOffset = 0;
	LPVOID * pTemp;

	CString strFileName = CUtility::GetCurrentDirectory();
	strFileName.Append( _T("cache\\") );
	::CreateDirectory( strFileName, NULL);

	strFileName.AppendFormat( _T("%s.cache"), Inspection::GetFileCacheName());

	CString strContent;
	strContent.Append(Inspection::GetFileCacheName());


	SAVE_OFFSET(MethodDesc::s_pfnReset);
	SAVE_OFFSET(MethodDesc::s_pfnIsGenericMethodDefinition);
	SAVE_OFFSET(MethodDesc::s_pfnGetNumGenericMethodArgs);
	SAVE_OFFSET(MethodDesc::s_pfnStripMethodInstantiation);
	SAVE_OFFSET(MethodDesc::s_pfnHasClassOrMethodInstantiation);
	SAVE_OFFSET(MethodDesc::s_pfnContainsGenericVariables);
	SAVE_OFFSET(MethodDesc::s_pfnGetWrappedMethodDesc);
	SAVE_OFFSET(MethodDesc::s_pfnGetDomain);
	SAVE_OFFSET(MethodDesc::s_pfnGetLoaderModule);

	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnConstructor);
	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnConstructor_v45);
	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnConstructor_v46);
	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnStart);
	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnNext_v4);
	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnNext_v2);
	SAVE_OFFSET(LoadedMethodDescIterator::s_pfnCurrent);


	CAtlFile file;
	HRESULT hr = file.Create( strFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, CREATE_ALWAYS);
	if( SUCCEEDED(hr) )
	{
		
		CHAR * pStr = T2A(strContent);
		int nLen = strlen(pStr);
		DWORD dwWritten = 0;
		file.Write( pStr, nLen, &dwWritten);
		file.Flush();
		file.Close();
	}
}