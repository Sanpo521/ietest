#pragma once

//��ӡ����JSON����󳤶�
#define MAX_JSON				1000

BOOL GetDefaultPrinterInfo(TCHAR* szPrinterName);
BOOL GetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting);
BOOL SetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting);