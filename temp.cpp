//// MySetPrinter
//// 
//// Demonstrates how to use the SetPrinter API.  This particular function changes the orienation
//// for the printer specified in pszPrinterName to the orientation specified in dmOrientation.
//// 
//// Valid values for dmOrientation are:
//// DMORIENT_PORTRAIT (1)
//// or
//// DMORIENT_LANDSCAPE (2)
//BOOL MySetPrinter(LPTSTR pszPrinterName, short dmOrientation)
//{
//    HANDLE hPrinter = NULL;
//    DWORD dwNeeded = 0;
//    PRINTER_INFO_2* pi2 = NULL;
//    DEVMODE* pDevMode = NULL;
//    PRINTER_DEFAULTS pd;
//    BOOL bFlag;
//    LONG lFlag;
//
//    // Open printer handle (on Windows NT, you need full-access because you will eventually use SetPrinter)...
//    ZeroMemory(&pd, sizeof(pd));
//    pd.DesiredAccess = PRINTER_ALL_ACCESS;
//    bFlag = OpenPrinter(pszPrinterName, &hPrinter, &pd);
//    if (!bFlag || (hPrinter == NULL))
//        return FALSE;
//
//    // The first GetPrinter tells you how big the buffer should be in 
//    // order to hold all of PRINTER_INFO_2. Note that this should fail with 
//    // ERROR_INSUFFICIENT_BUFFER.  If GetPrinter fails for any other reason 
//    // or dwNeeded isn't set for some reason, then there is a problem...
//    SetLastError(0);
//    bFlag = GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
//    if ((!bFlag) && (GetLastError() != ERROR_INSUFFICIENT_BUFFER) ||
//        (dwNeeded == 0))
//    {
//        ClosePrinter(hPrinter);
//        return FALSE;
//    }
//
//    // Allocate enough space for PRINTER_INFO_2...
//    pi2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR, dwNeeded);
//    if (pi2 == NULL)
//    {
//        ClosePrinter(hPrinter);
//        return FALSE;
//    }
//
//    // The second GetPrinter fills in all the current settings, so all you
//    // need to do is modify what you're interested in...
//    bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
//    if (!bFlag)
//    {
//        GlobalFree(pi2);
//        ClosePrinter(hPrinter);
//        return FALSE;
//    }
//
//    // If GetPrinter didn't fill in the DEVMODE, try to get it by calling DocumentProperties...
//    if (pi2->pDevMode == NULL)
//    {
//        dwNeeded = DocumentProperties(NULL, hPrinter,
//            pszPrinterName,
//            NULL, NULL, 0);
//        if (dwNeeded <= 0)
//        {
//            GlobalFree(pi2);
//            ClosePrinter(hPrinter);
//            return FALSE;
//        }
//
//        pDevMode = (DEVMODE*)GlobalAlloc(GPTR, dwNeeded);
//        if (pDevMode == NULL)
//        {
//            GlobalFree(pi2);
//            ClosePrinter(hPrinter);
//            return FALSE;
//        }
//
//        lFlag = DocumentProperties(NULL, hPrinter,
//            pszPrinterName,
//            pDevMode, NULL,
//            DM_OUT_BUFFER);
//        if (lFlag != IDOK || pDevMode == NULL)
//        {
//            GlobalFree(pDevMode);
//            GlobalFree(pi2);
//            ClosePrinter(hPrinter);
//            return FALSE;
//        }
//
//        pi2->pDevMode = pDevMode;
//    }
//
//    // Driver is reporting that it doesn't support this change...
//    if (!(pi2->pDevMode->dmFields & DM_ORIENTATION))
//    {
//        GlobalFree(pi2);
//        ClosePrinter(hPrinter);
//        if (pDevMode)
//            GlobalFree(pDevMode);
//        return FALSE;
//    }
//
//    // Specify exactly what we are attempting to change...
//    pi2->pDevMode->dmFields = DM_ORIENTATION;
//    pi2->pDevMode->dmOrientation = dmOrientation;
//
//    // Do not attempt to set security descriptor...
//    pi2->pSecurityDescriptor = NULL;
//
//    // Make sure the driver-dependent part of devmode is updated...
//    lFlag = DocumentProperties(NULL, hPrinter,
//        pszPrinterName,
//        pi2->pDevMode, pi2->pDevMode,
//        DM_IN_BUFFER | DM_OUT_BUFFER);
//    if (lFlag != IDOK)
//    {
//        GlobalFree(pi2);
//        ClosePrinter(hPrinter);
//        if (pDevMode)
//            GlobalFree(pDevMode);
//        return FALSE;
//    }
//
//    // Update printer information...
//    bFlag = SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);
//    if (!bFlag)
//        // The driver doesn't support, or it is unable to make the change...
//    {
//        GlobalFree(pi2);
//        ClosePrinter(hPrinter);
//        if (pDevMode)
//            GlobalFree(pDevMode);
//        return FALSE;
//    }
//
//    // Tell other apps that there was a change...
//    SendMessageTimeout(HWND_BROADCAST, WM_DEVMODECHANGE, 0L,
//        (LPARAM)(LPCSTR)pszPrinterName,
//        SMTO_NORMAL, 1000, NULL);
//
//    // Clean up...
//    if (pi2)
//        GlobalFree(pi2);
//    if (hPrinter)
//        ClosePrinter(hPrinter);
//    if (pDevMode)
//        GlobalFree(pDevMode);
//
//    return TRUE;
//}