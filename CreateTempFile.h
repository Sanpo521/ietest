#pragma once

#define BUFSIZE 1024

//void PrintError(LPCTSTR errDesc);
VOID CreateTempFile(TCHAR* szFileContent, TCHAR* szTempFileName);
VOID GetTempHtmlFileName(TCHAR* szTempFileName);
HANDLE CreateTempHtmlFile(TCHAR* szTempFileName);
VOID WriteTempHtmlFile(HANDLE hTempFile, TCHAR* szFileContent);
//VOID CloseTempHtmlFile(HANDLE hTempFile);