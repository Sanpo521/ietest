#pragma once

//打印设置JSON串最大长度
#define MAX_JSON				1000

BOOL GetDefaultPrinterInfo(TCHAR* szPrinterName);
BOOL GetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting);
BOOL SetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting);