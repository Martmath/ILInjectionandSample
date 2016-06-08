#pragma once
#include <map>
class CSymbolAddressCache
{
public:
	static BOOL TryCache(void);	
	static void GenerateClrCache(void);

private:

	static std::map<CString, ULONG64> LoadCache(LPCTSTR lpszMd5Hash);
	static BOOL TryLoadPredefinedCache(LPCTSTR lpszMd5Hash, std::map<CString, ULONG64> & mpOffset);
	static BOOL CSymbolAddressCache::ParseContent(CString strContent, std::map<CString, ULONG64> & mpOffset);
};

