#pragma once
BOOL GetPrinterDevice(LPTSTR pszPrinterName, HGLOBAL* phDevNames, HGLOBAL* phDevMode, LPTSTR pDriverName, LPTSTR pPortName, LPTSTR pPrinterName);
BOOL GetPrinterDeviceEx(LPTSTR pszPrinterName, TCHAR* phDevNames, TCHAR* phDevMode, LPTSTR pDriverName, LPTSTR pPortName, LPTSTR pPrinterName);