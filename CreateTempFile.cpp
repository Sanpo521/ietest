#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "CreateTempFile.h"

VOID GetTempHtmlFileName(TCHAR* szTempFileName) {
    DWORD dwRetVal = 0;
    UINT uRetVal = 0;
    DWORD dwBytesWritten = 0;
    TCHAR lpTempPathBuffer[MAX_PATH];

    //ȡ����ʱ·��
    dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
        lpTempPathBuffer); // buffer for path  
    if (dwRetVal > MAX_PATH || (dwRetVal == 0))
    {
        PrintError(TEXT("GetTempPath failed"));
        return;
    }

    //ȡ����ʱ�ļ��� 
    uRetVal = GetTempFileName(lpTempPathBuffer, // directory for tmp files
        TEXT("TopTP"),     // temp file name prefix 
        0,                // create unique name 
        szTempFileName);  // buffer for name 
    if (uRetVal == 0)
    {
        PrintError(TEXT("GetTempFileName failed"));
        return;
    }
}

HANDLE CreateTempHtmlFile(TCHAR* szTempFileName) {
    HANDLE hTempFile = INVALID_HANDLE_VALUE;

    //��������д�����ʱ�ļ�
    hTempFile = CreateFile((LPTSTR)szTempFileName, // file name 
        GENERIC_WRITE | GENERIC_READ,          // д�Ͷ��ļ���
        0,                      // �������д��
        NULL,                   // ȱʡ��ȫ���ԡ�
        OPEN_ALWAYS,          // OPEN_ALWAYS  ���ļ����������򴴽���    OPEN_EXISTING ���ļ����������򱨴�
        FILE_ATTRIBUTE_NORMAL, // һ����ļ���       
        NULL);                 // ģ���ļ�Ϊ�ա�
    if (hTempFile == INVALID_HANDLE_VALUE)
    {
        PrintError(TEXT("Second CreateFile failed"));
    }
    return hTempFile;
}

VOID WriteTempHtmlFile(HANDLE hTempFile, TCHAR* szFileContent) {
    BOOL fSuccess = FALSE;
    DWORD dwBytesWritten = 0;

    char* utf8 = (char*)malloc(10485760);
    int lBytesWritten = WideCharToMultiByte(CP_UTF8, 0, szFileContent, -1, utf8, lstrlen(szFileContent) * sizeof(TCHAR), NULL, NULL);

    //����ƫ���� ���ļ�β�� ���OPEN_EXISTINGʹ�� ��ʵ��׷��д���ļ�
    SetFilePointer(hTempFile, NULL, NULL, FILE_END);

    fSuccess = WriteFile(hTempFile, //��ʱ�ļ����
        utf8,
        strlen(utf8),//lstrlen(szFileContent) * sizeof(TCHAR),
        &dwBytesWritten,
        NULL);
    if (!fSuccess)
    {
        PrintError(TEXT("WriteFile failed"));
        return;
    }
    free(utf8);
    SetEndOfFile(hTempFile);
    if (!CloseHandle(hTempFile))
    {
        PrintError(TEXT("CloseHandle(hTempFile) failed"));
        return;
    }
}

VOID CreateTempFile(TCHAR* szTempFileName, TCHAR* szFileContent) {
    HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE hTempFile = INVALID_HANDLE_VALUE;

    BOOL fSuccess = FALSE;
    DWORD dwRetVal = 0;
    UINT uRetVal = 0;

    DWORD dwBytesRead = 0;
    DWORD dwBytesWritten = 0;

    //TCHAR szTempFileName[MAX_PATH];
    TCHAR lpTempPathBuffer[MAX_PATH];
    char  chBuffer[BUFSIZE];

    LPCTSTR errMsg;

    //ȡ����ʱ·��
    dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
                           lpTempPathBuffer); // buffer for path  
    if (dwRetVal > MAX_PATH || (dwRetVal == 0))
    {
        PrintError(TEXT("GetTempPath failed"));
        return;
    }

    //ȡ����ʱ�ļ��� 
    uRetVal = GetTempFileName(lpTempPathBuffer, // directory for tmp files
                                TEXT(".htm"),     // temp file name prefix 
                                0,                // create unique name 
                                szTempFileName);  // buffer for name 
    if (uRetVal == 0)
    {
        PrintError(TEXT("GetTempFileName failed"));
        return;
    }

    //��������д�����ʱ�ļ�
    hTempFile = CreateFile((LPTSTR)szTempFileName, // file name 
                            GENERIC_WRITE,        // open for write 
                            0,                    // do not share 
                            NULL,                 // default security 
                            CREATE_ALWAYS,        // overwrite existing
                            FILE_ATTRIBUTE_NORMAL,// normal file 
                            NULL);                // no template 
    if (hTempFile == INVALID_HANDLE_VALUE)
    {
        PrintError(TEXT("Second CreateFile failed"));
        return;
    }

    char* utf8 = (char*)malloc(10485760);
    int lBytesWritten = WideCharToMultiByte(CP_UTF8, 0, szFileContent, -1, utf8, lstrlen(szFileContent) * sizeof(TCHAR), NULL, NULL);

    fSuccess = WriteFile(hTempFile, //��ʱ�ļ����
                        utf8,
                        strlen(utf8),//lstrlen(szFileContent) * sizeof(TCHAR),
                        &dwBytesWritten,
                        NULL);
    if (!fSuccess)
    {
        PrintError(TEXT("WriteFile failed"));
        return;
    }

    //// lBytesWritten includes the null
    //int err = GetLastError();
    //WriteFile(hFile, utf8, lBytesWritten, &dwBytesWritten, NULL);
    free(utf8);
    SetEndOfFile(hTempFile);


    if (!CloseHandle(hTempFile))
    {
        PrintError(TEXT("CloseHandle(hTempFile) failed"));
        return;
    }
}

//  ErrorMessage support function.
//  Retrieves the system error message for the GetLastError() code.
//  Note: caller must use LocalFree() on the returned LPCTSTR buffer.
LPCTSTR ErrorMessage(DWORD error)
{
    LPVOID lpMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL);

    return((LPCTSTR)lpMsgBuf);
}

//  PrintError support function.
//  Simple wrapper function for error output.
void PrintError(LPCTSTR errDesc)
{
    LPCTSTR errMsg = ErrorMessage(GetLastError());
    _tprintf(TEXT("\n** ERROR ** %s: %s\n"), errDesc, errMsg);
    LocalFree((LPVOID)errMsg);
}