#pragma once
class CUtility
{
public:
	static CString ComputeMd5HashForModule(HMODULE hModule);

	static BOOL GetFileVersion(LPWSTR lpszFile, VS_FIXEDFILEINFO * pFixedFileInfo);

	static CString GetFileName(LPCTSTR lpszFilePath);

	static CString GetCurrentDirectory(void);

	static HMODULE GetCurrentModule(void);
};

