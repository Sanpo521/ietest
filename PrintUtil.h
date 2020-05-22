#pragma once
BOOL GetDefaultPrinterInfo(TCHAR* szPrinterName);
BOOL GetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting);
BOOL SetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting);