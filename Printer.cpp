#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlstr.h>
#include <cassert>
#include "Printer.h"

BOOL GetPrinterDeviceEx(LPTSTR pszPrinterName, TCHAR* phDevNames, TCHAR* phDevMode, LPTSTR pDriverName, LPTSTR pPortName, LPTSTR pPrinterName)
{
    // if NULL is passed, then assume we are setting app object's
    // devmode and devnames
    if (phDevMode == NULL || phDevNames == NULL)
        return FALSE;

    // Open printer
    HANDLE hPrinter;
    if (OpenPrinter(pszPrinterName, &hPrinter, NULL) == FALSE)
        return FALSE;

    //LONG lngPrinter = ::DocumentProperties(0, hPrinter, pszPrinterName, NULL, NULL, 0);
    //if (lngPrinter < 0)
    //{
    //    return FALSE;
    //}
    //HGLOBAL  hDevMode = GlobalAlloc(GHND, lngPrinter);
    //DEVMODE* pDevMode = (DEVMODE*)GlobalLock(hDevMode);

    // obtain PRINTER_INFO_2 structure and close printer
    DWORD dwBytesReturned, dwBytesNeeded;
    GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);
    PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR, dwBytesNeeded);
    if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded, &dwBytesReturned) == 0) {
        GlobalFree(p2);
        ClosePrinter(hPrinter);
        return FALSE;
    }
    ClosePrinter(hPrinter);

    // Allocate a global handle for DEVMODE
    HGLOBAL  hDevMode = GlobalAlloc(GHND, sizeof(*p2->pDevMode) + p2->pDevMode->dmDriverExtra);
    assert(hDevMode);
    DEVMODE* pDevMode = (DEVMODE*)GlobalLock(hDevMode);
    assert(pDevMode);

    // copy DEVMODE data from PRINTER_INFO_2::pDevMode
    memcpy(pDevMode, p2->pDevMode, sizeof(*p2->pDevMode) + p2->pDevMode->dmDriverExtra);
    GlobalUnlock(hDevMode);


    // Compute size of DEVNAMES structure from PRINTER_INFO_2's data
    DWORD drvNameLen = lstrlen(p2->pDriverName) + 1;  // driver name
    DWORD ptrNameLen = lstrlen(p2->pPrinterName) + 1; // printer name
    DWORD porNameLen = lstrlen(p2->pPortName) + 1;    // port name
                                                      // Allocate a global handle big enough to hold DEVNAMES.

    _tcscpy_s(pDriverName, drvNameLen, p2->pDriverName);
    _tcscpy_s(pPortName, porNameLen, p2->pPortName);
    _tcscpy_s(pPrinterName, ptrNameLen, p2->pPrinterName);

    HGLOBAL hDevNames = GlobalAlloc(GHND, sizeof(DEVNAMES) + (drvNameLen + ptrNameLen + porNameLen) * sizeof(TCHAR));
    assert(hDevNames);
    DEVNAMES* pDevNames = (DEVNAMES*)GlobalLock(hDevNames);
    assert(pDevNames);

       // Copy the DEVNAMES information from PRINTER_INFO_2
    // tcOffset = TCHAR Offset into structure
    int tcOffset = sizeof(DEVNAMES) / sizeof(TCHAR);
    assert(sizeof(DEVNAMES) == tcOffset * sizeof(TCHAR));

    pDevNames->wDriverOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pDriverName, drvNameLen * sizeof(TCHAR));
    tcOffset += drvNameLen;

    pDevNames->wDeviceOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pPrinterName, ptrNameLen * sizeof(TCHAR));
    tcOffset += ptrNameLen;

    pDevNames->wOutputOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pPortName, porNameLen * sizeof(TCHAR));
    pDevNames->wDefault = 0;

    GlobalUnlock(hDevNames);
    GlobalFree(p2);   // free PRINTER_INFO_2

    // set the new hDevMode and hDevNames

    _stprintf_s(phDevMode, MAX_PATH, _T("0x%p"), hDevMode);
    _stprintf_s(phDevNames, MAX_PATH, _T("0x%p"), hDevNames);
    //*phDevMode = hDevMode;
    //*phDevNames = hDevNames;
    return TRUE;
}

BOOL GetPrinterDevice(LPTSTR pszPrinterName, HGLOBAL* phDevNames, HGLOBAL* phDevMode, LPTSTR pDriverName, LPTSTR pPortName, LPTSTR pPrinterName)
{
    // if NULL is passed, then assume we are setting app object's
    // devmode and devnames
    if (phDevMode == NULL || phDevNames == NULL)
        return FALSE;

    // Open printer
    HANDLE hPrinter;
    if (OpenPrinter(pszPrinterName, &hPrinter, NULL) == FALSE)
        return FALSE;

    // obtain PRINTER_INFO_2 structure and close printer
    DWORD dwBytesReturned, dwBytesNeeded;
    GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);
    PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,  dwBytesNeeded);
    if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded, &dwBytesReturned) == 0) {
        GlobalFree(p2);
        ClosePrinter(hPrinter);
        return FALSE;
    }
    ClosePrinter(hPrinter);

    //::DocumentProperties(0, hPrinter, pszPrinterName, )

    // Allocate a global handle for DEVMODE
    HGLOBAL  hDevMode = GlobalAlloc(GHND, sizeof(*p2->pDevMode) + p2->pDevMode->dmDriverExtra);
    assert(hDevMode);
    DEVMODE* pDevMode = (DEVMODE*)GlobalLock(hDevMode);
    assert(pDevMode);

    // copy DEVMODE data from PRINTER_INFO_2::pDevMode
    memcpy(pDevMode, p2->pDevMode, sizeof(*p2->pDevMode) + p2->pDevMode->dmDriverExtra);
    GlobalUnlock(hDevMode);

    // Compute size of DEVNAMES structure from PRINTER_INFO_2's data
    DWORD drvNameLen = lstrlen(p2->pDriverName) + 1;  // driver name
    DWORD ptrNameLen = lstrlen(p2->pPrinterName) + 1; // printer name
    DWORD porNameLen = lstrlen(p2->pPortName) + 1;    // port name
                                                      // Allocate a global handle big enough to hold DEVNAMES.

	_tcscpy_s(pDriverName, drvNameLen, p2->pDriverName);
    _tcscpy_s(pPortName, porNameLen, p2->pPortName);
    _tcscpy_s(pPrinterName, ptrNameLen, p2->pPrinterName);

    HGLOBAL hDevNames = GlobalAlloc(GHND, sizeof(DEVNAMES) + (drvNameLen + ptrNameLen + porNameLen) * sizeof(TCHAR));
    assert(hDevNames);
    DEVNAMES* pDevNames = (DEVNAMES*)GlobalLock(hDevNames);
    assert(pDevNames);

    // Copy the DEVNAMES information from PRINTER_INFO_2
    // tcOffset = TCHAR Offset into structure
    int tcOffset = sizeof(DEVNAMES) / sizeof(TCHAR);
    assert(sizeof(DEVNAMES) == tcOffset * sizeof(TCHAR));

    pDevNames->wDriverOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pDriverName,  drvNameLen * sizeof(TCHAR));
    tcOffset += drvNameLen;

    pDevNames->wDeviceOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pPrinterName, ptrNameLen * sizeof(TCHAR));
    tcOffset += ptrNameLen;

    pDevNames->wOutputOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pPortName, porNameLen * sizeof(TCHAR));
    pDevNames->wDefault = 0;

    GlobalUnlock(hDevNames);
    GlobalFree(p2);   // free PRINTER_INFO_2

    // set the new hDevMode and hDevNames
    *phDevMode = &hDevMode;
    *phDevNames = &hDevNames;
    return TRUE;
}
