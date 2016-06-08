#include "StdAfx.h"
#include "Utility.h"
#include <wincrypt.h>

#define BUFSIZE 1024
#define MD5LEN  16


#if defined(_WIN64)
#define MODULE_NAME _T("Injection64.dll")
#else
#define MODULE_NAME _T("Injection32.dll")
#endif









CString CUtility::ComputeMd5HashForModule(HMODULE hModule)
{
	CString strRet;
	
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	WCHAR wszPath[MAX_PATH] = {0};
	::GetModuleFileNameW( hModule, wszPath, MAX_PATH);

	HANDLE hFile = CreateFileW(wszPath
		, GENERIC_READ
		, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		, NULL
		, OPEN_EXISTING
		, FILE_FLAG_SEQUENTIAL_SCAN
		, NULL
		);
	if( hFile == INVALID_HANDLE_VALUE )
		return strRet;

	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		CloseHandle(hFile);
		return strRet;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return strRet;
	}

	BOOL bResult = FALSE;
	DWORD dwRead = 0;
	BYTE szBuffer[BUFSIZE];
	while (bResult = ReadFile(hFile, szBuffer, BUFSIZE, &dwRead, NULL))
	{
		if (0 == dwRead)
		{
			break;
		}

		if (!CryptHashData(hHash, szBuffer, dwRead, 0))
		{
			bResult = FALSE;
			break;
		}
	}
	CloseHandle(hFile);

	if (bResult)
	{
		BYTE szHash[MD5LEN];
		DWORD dwHash = sizeof(szHash);
		if (CryptGetHashParam(hHash, HP_HASHVAL, szHash, &dwHash, 0))
		{
			TCHAR tszDigits[] = _T("0123456789ABCDEF");
			for (DWORD i = 0; i < dwHash; i++)
			{
				strRet.AppendFormat( _T("%c%c"), tszDigits[szHash[i] >> 4], tszDigits[szHash[i] & 0xF]);
			}
		}
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	return strRet;	
}


BOOL CUtility::GetFileVersion(LPWSTR lpszFile, VS_FIXEDFILEINFO * pFixedFileInfo)
{
	if( !pFixedFileInfo )
		return FALSE;

	DWORD  dwHandle = NULL;
	UINT   nSize = 0;
	
	
	DWORD  dwSize = GetFileVersionInfoSizeW( lpszFile, &dwHandle);
	if( dwSize == 0 )
		return FALSE;
	
	BYTE   szTempBuf[4096] = {0};
	LPVOID pData = szTempBuf;
	ATLVERIFY( dwSize < sizeof(szTempBuf) );

	if (GetFileVersionInfo( lpszFile, dwHandle, dwSize, pData))
	{
		LPBYTE pBuffer = NULL;
		if (VerQueryValueW( pData, L"\\",(VOID FAR* FAR*)&pBuffer,&nSize))
		{
			if (nSize > 0)
			{
				VS_FIXEDFILEINFO * pVerInfo = (VS_FIXEDFILEINFO *)pBuffer;
				memcpy_s( pFixedFileInfo, sizeof(VS_FIXEDFILEINFO), pBuffer, sizeof(VS_FIXEDFILEINFO));
				return TRUE;
			}
		}
	}
	return FALSE;
}


CString CUtility::GetFileName(LPCTSTR lpszFilePath)
{
	CString strRet(lpszFilePath);
	int nIndex = strRet.ReverseFind('\\');
	if( nIndex > 0 )
		strRet = strRet.Mid( nIndex + 1 );
	return strRet;
}


CString CUtility::GetCurrentDirectory(void)
{
	HMODULE hModule = GetCurrentModule();
	ATLVERIFY(hModule);

	TCHAR tszCurPath[MAX_PATH] = {0};
	{
		int nLen = ::GetModuleFileName( hModule, tszCurPath, MAX_PATH);
		for( ; nLen > 0; nLen --)
		{
			if( tszCurPath[nLen-1] == '\\' )
			{
				tszCurPath[nLen] = '\0';
				break;
			}
		}
	}
	return tszCurPath;
}


HMODULE CUtility::GetCurrentModule(void)
{
	return ::GetModuleHandle(MODULE_NAME);
}