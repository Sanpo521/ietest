#include <atlbase.h>
#include <json/json.h>
#include "DebugInfoPrint.h"
#include "PrintUtil.h"
#include "StringUtil.h"

//PDevMode = _devicemodeW;
//  _devicemodeW = record
//    dmDeviceName: array[0..CCHDEVICENAME - 1] of WideChar; {打印设备名称,唯一值}
//    dmSpecVersion: Word;  {DevMode的版本号}
//    dmDriverVersion: Word;  {打印驱动程序版本号}
//    dmSize: Word; {DevMode结构的字节大小}
//    dmDriverExtra: Word; {dmDriverExtra成员结构的大小}
//    dmFields: DWORD; {DevMode结构中哪些成员已被初始化}
//    dmOrientation: SHORT; {纸张方向;1(纵向)2(横向)}
//    dmPaperSize: SHORT; {纸张大小}
//    dmPaperLength: SHORT; {自定义纸张的高度,PageSize为0或256时有效}
//    dmPaperWidth: SHORT; {自定义纸张的宽度,PageSize为0或256时有效}
//    dmScale: SHORT; {打印输出的缩放系数}
//    dmCopies: SHORT; {打印份数}
//    dmDefaultSource: SHORT; {送纸的默认纸盒}
//    dmPrintQuality: SHORT; {打印机的分辨率;4-高品质 3-中品质 2-低品质 1-草稿}
//    dmColor: SHORT; {打印输出为彩色或是单色;1-彩色 2-单色}
//    dmDuplex: SHORT; {纸张是否为双面打印输出;1-单面 2-水平 3-垂直(需打印支持)}
//    dmYResolution: SHORT; {Y方向分辨率;单位:点}
//    dmTTOption: SHORT; {如何打印TrueType字体}
//    dmCollate: SHORT; {多份时,是否逐份打印}
//    dmFormName: array[0..CCHFORMNAME - 1] of WideChar; {纸张名称}
//    dmLogPixels: Word; {}
//    dmBitsPerPel: DWORD; {显示设备的颜色分辨率;单位:像素}
//    dmPelsWidth: DWORD; {可见设备表面(屏幕或打印机)的宽度}
//    dmPelsHeight: DWORD;  {可见设备表面(屏幕或打印机)的高度}
//    dmDisplayFlags: DWORD; {设备的显示模式}
//    dmDisplayFrequency: DWORD; {显示设备在特定模式下的频率}
//    dmICMMethod: DWORD; {}
//    dmICMIntent: DWORD; {}
//    dmMediaType: DWORD; {}
//    dmDitherType: DWORD; {}
//    dmICCManufacturer: DWORD; {}
//    dmICCModel: DWORD; {}
//    dmPanningWidth: DWORD; {平移宽度}
//    dmPanningHeight: DWORD; {平移高度}
//  end;



//目录	1
//一、简介	2
//二、webprint插件的对象、属性方法	2
//1、对象	2
//2、属性	3
//defaultPrinterName	3
//
//pageFrom	6
//pageTo	6
//selectedPages	6
//currentPage	7
//zoomValue	8
//
//3、方法	9
//InitPrint()	9
//SetMarginMeasure(int units)	9
//Preview()	10
//Print([bool] prompt)	10
//三、例子摘要	10
//设置页边距的打印示例	10
//选择打印纸型的示例	12
//自定义打印纸张的示例	13



/******************************************************************************************
Author	    :	Sanpo
Date	    :	2020-05-18
Description :	打印工具类（包含取得默认打印机、获取打印机设置、修改打印机设置等
******************************************************************************************/

/******************************************************************************************
Name	            :	GetDefaultPrinterInfo
Date	            :	2020-05-18
Description         :	获取默认打印机名称
Input parameters    :
Output parameters   :   pszPrinterName 默认打印机名称指针
******************************************************************************************/
BOOL GetDefaultPrinterInfo(TCHAR* pszPrinterName) {
	DWORD dwSize = 0;
	int result = 0;
	if (!::GetDefaultPrinter(NULL, &dwSize) && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		result = ::GetDefaultPrinter(pszPrinterName, &dwSize);
		return TRUE;
	}

	if (result == ERROR_INSUFFICIENT_BUFFER)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetDefaultPrinterInfo ERROR_INSUFFICIENT_BUFFER the real size is:%d"), dwSize);
	}
	else if (result == ERROR_FILE_NOT_FOUND) {
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetDefaultPrinterInfo ERROR_FILE_NOT_FOUND"));
	}
	else
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetDefaultPrinterInfo GetLastError:%d"), result);
	}
	return FALSE;
}

/******************************************************************************************
Name	            :	GetPrintSettingInfo
Date	            :	2020-05-18
Description         :	获取打印机设置
Input parameters    :   hPrinter        打印机句柄
						pszPrinterName  打印机名称指针
Output parameters   :   pszPrintSetting 获取打印机设置JSON
							paperSize	打印纸张
								8   A3 297 x 420 mm
								9   A4 210 x 297 mm
								256 Custom
							pageWidth	打印页面的宽,单位为毫米
							pageHeight	打印页面的高,单位为毫米
							orientation	打印方向，横向或者纵向 1为纵向，2为横向
							copies	    打印的份数
							paperSource 纸张来源
******************************************************************************************/
BOOL GetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting) {
	DWORD dwSize = 0;

	// Open printer
	HANDLE hPrinter;
	if (!::OpenPrinter(pszPrinterName, &hPrinter, NULL)) {
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetPrintSettingInfo OpenPrinter [%s] fails"), pszPrinterName);
		return FALSE;
	}

	LONG lngPrinter = ::DocumentProperties(0, hPrinter, pszPrinterName, NULL, NULL, 0);
	if (lngPrinter < 0)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetPrintSettingInfo DocumentProperties1 [%s] fails"), pszPrinterName);
		::ClosePrinter(hPrinter);
		return FALSE;
	}
	DWORD    dwRet = 0;
	DEVMODE* pDevMode = (DEVMODE*)::GlobalAlloc(GPTR, lngPrinter);
	dwRet = ::DocumentProperties(0, hPrinter, pszPrinterName, pDevMode, NULL, DM_OUT_BUFFER);
	if (dwRet != IDOK)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetPrintSettingInfo DocumentProperties2 [%s] fails"), pszPrinterName);
		if (hPrinter) {
			::ClosePrinter(hPrinter);
		}
		if (pDevMode) {
			::GlobalFree(pDevMode);
		}
		return FALSE;
	}
	Json::Value jsonRoot; //定义根节点
	Json::Value jsonItem; //定义一个子对象
	jsonRoot["paperSize"] = Json::Value(pDevMode->dmPaperSize); //添加数据
	jsonRoot["paperHeight"] = Json::Value(pDevMode->dmPaperLength);
	jsonRoot["paperWidth"] = Json::Value(pDevMode->dmPaperWidth);
	jsonRoot["orientation"] = Json::Value(pDevMode->dmOrientation);
	jsonRoot["copies"] = Json::Value(pDevMode->dmCopies);
	jsonRoot["paperSource"] = Json::Value(pDevMode->dmDefaultSource);

	std::wstring wszDest;
	StringToWstring(wszDest, jsonRoot.toStyledString());
	const wchar_t* pwidstr = wszDest.c_str();
	_tcscpy_s(pszPrintSetting, MAX_JSON, pwidstr);
	if (hPrinter) {
		::ClosePrinter(hPrinter);
	}
	if (pDevMode) {
		::GlobalFree(pDevMode);
	}

	return TRUE;
}

/******************************************************************************************
Name	            :	SetPrintSettingInfo
Date	            :	2020-05-18
Description         :	设置打印机设置
Input parameters    :   hPrinter        打印机句柄
						pszPrinterName  打印机名称指针
						pszPrintSetting 设置打印机设置JSON
							paperSize	设置打印纸张(设置值必须是打印机直接的纸型)
								8   A3 297 x 420 mm
								9   A4 210 x 297 mm
								256 Custom
							pageWidth	设置打印页面的宽,单位为毫米 如果paperSize不为256，该值将忽略
							pageHeight	设置打印页面的高,单位为毫米 如果paperSize不为256，该值将忽略
							orientation	设置打印方向，横向或者纵向 1为纵向，2为横向
							copies	    设置打印的份数
							paperSource 设置纸张来源
******************************************************************************************/
BOOL SetPrintSettingInfo(TCHAR* pszPrinterName, TCHAR* pszPrintSetting) {
	std::string jsonStr;
	WcharToString(jsonStr, pszPrintSetting);

	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(jsonStr, root))
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo json parse [%s] fails"), pszPrintSetting);
		return FALSE;
	}

	HANDLE hPrinter = NULL;
	DWORD dwNeeded = 0;
	PRINTER_INFO_2* pi2 = NULL;
	DEVMODE* pDevMode = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	LONG lFlag;

	// Open printer handle (on Windows NT, you need full-access because you will eventually use SetPrinter)...
	::ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	bFlag = OpenPrinter(pszPrinterName, &hPrinter, &pd);
	if (!bFlag || (hPrinter == NULL)) {
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo OpenPrinter [%s] fails"), pszPrinterName);
		return FALSE;
	}

	//第一个GetPrinter告诉您缓冲区的大小，以便容纳所有PRINTER_INFO_2。 
	//请注意，这将失败并显示ERROR_INSUFFICIENT_BUFFER。 
	//如果GetPrinter由于任何其他原因而失败，或者由于某些原因未设置dwNeeded，则存在问题...
	SetLastError(0);
	bFlag = GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
	if ((!bFlag) && (GetLastError() != ERROR_INSUFFICIENT_BUFFER) || (dwNeeded == 0))
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo GetPrinter1 [%d] fails"), GetLastError());
		ClosePrinter(hPrinter);
		return FALSE;
	}

	// Allocate enough space for PRINTER_INFO_2...
	pi2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR, dwNeeded);
	if (pi2 == NULL)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo GlobalAlloc1 [%d] fails"), GetLastError());
		ClosePrinter(hPrinter);
		return FALSE;
	}

	//第二个GetPrinter填写所有当前设置，因此您所需要做的就是修改您感兴趣的内容...
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo GetPrinter2 [%d] fails"), GetLastError());
		GlobalFree(pi2);
		ClosePrinter(hPrinter);
		return FALSE;
	}

	//如果GetPrinter没有填写DEVMODE，请尝试通过调用DocumentProperties来获取它。
	if (pi2->pDevMode == NULL)
	{
		dwNeeded = DocumentProperties(NULL, hPrinter, pszPrinterName, NULL, NULL, 0);
		if (dwNeeded <= 0)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo DocumentProperties1 [%d] fails"), GetLastError());
			GlobalFree(pi2);
			ClosePrinter(hPrinter);
			return FALSE;
		}

		pDevMode = (DEVMODE*)GlobalAlloc(GPTR, dwNeeded);
		if (pDevMode == NULL)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo GlobalAlloc2 [%d] fails"), GetLastError());
			GlobalFree(pi2);
			ClosePrinter(hPrinter);
			return FALSE;
		}

		lFlag = DocumentProperties(NULL, hPrinter, pszPrinterName, pDevMode, NULL, DM_OUT_BUFFER);
		if (lFlag != IDOK || pDevMode == NULL)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo DocumentProperties2 [%d] fails"), GetLastError());
			GlobalFree(pDevMode);
			GlobalFree(pi2);
			ClosePrinter(hPrinter);
			return FALSE;
		}
		pi2->pDevMode = pDevMode;
	}

	bool isChange = false;
	//根据驱动程序报告判断要修改的属性支持不支持更改
	//如果支持，则更改的内容
	if (!root["paperSize"].isNull()) {
		if (pi2->pDevMode->dmFields & DM_PAPERSIZE)
		{
			pi2->pDevMode->dmFields = pi2->pDevMode->dmFields | DM_PAPERSIZE;
			pi2->pDevMode->dmPaperSize = root["paperSize"].asInt();
			isChange = true;
			if (pi2->pDevMode->dmPaperSize == DMPAPER_USER) {
				if (!root["paperHeight"].isNull()) {
					if (pi2->pDevMode->dmFields & DM_PAPERLENGTH)
					{
						pi2->pDevMode->dmFields = pi2->pDevMode->dmFields | DM_PAPERLENGTH;
						pi2->pDevMode->dmPaperLength = root["paperHeight"].asInt();
						isChange = true;
					}
					else {
						OutputDebugPrintf(_T("[SanpoWebPrintWS] Printer doesn't support paperHeight"));
					}
				}
				if (!root["paperWidth"].isNull()) {
					if (pi2->pDevMode->dmFields & DM_PAPERWIDTH)
					{
						pi2->pDevMode->dmFields = pi2->pDevMode->dmFields | DM_PAPERWIDTH;
						pi2->pDevMode->dmPaperWidth = root["paperWidth"].asInt();
						isChange = true;
					}
					else {
						OutputDebugPrintf(_T("[SanpoWebPrintWS] Printer doesn't support paperWidth"));
					}
				}
			}
		}
		else {
			OutputDebugPrintf(_T("[SanpoWebPrintWS] Printer doesn't support paperSize"));
		}
	}
	if (!root["orientation"].isNull()) {
		if (pi2->pDevMode->dmFields & DM_ORIENTATION)
		{
			pi2->pDevMode->dmFields = pi2->pDevMode->dmFields | DM_ORIENTATION;
			pi2->pDevMode->dmOrientation = root["orientation"].asInt();
			isChange = true;
		}
		else {
			OutputDebugPrintf(_T("[SanpoWebPrintWS] Printer doesn't support Orientation"));
		}
	}
	if (!root["copies"].isNull()) {
		if (pi2->pDevMode->dmFields & DM_COPIES)
		{
			pi2->pDevMode->dmFields = pi2->pDevMode->dmFields | DM_COPIES;
			pi2->pDevMode->dmCopies = root["copies"].asInt();
			isChange = true;
		}
		else {
			OutputDebugPrintf(_T("[SanpoWebPrintWS] Printer doesn't support copies"));
		}
	}

	if (!isChange) {
		OutputDebugPrintf(_T("[SanpoWebPrintWS] The printer has no attributes to change"));
		GlobalFree(pi2);
		ClosePrinter(hPrinter);
		if (pDevMode) {
			GlobalFree(pDevMode);
		}
		return FALSE;
	}

	//不要尝试设置安全描述符.
	pi2->pSecurityDescriptor = NULL;

	//更新devmode的驱动程序相关部分
    lFlag = DocumentProperties(NULL, hPrinter, pszPrinterName, pi2->pDevMode, pi2->pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);
    if (lFlag != IDOK)
    {
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo DocumentProperties3 [%d] fails"), GetLastError());
        GlobalFree(pi2);
        ClosePrinter(hPrinter);
		if (pDevMode) {
			GlobalFree(pDevMode);
		}
        return FALSE;
    }
	
	//更新打印机信息
    bFlag = SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);
	//驱动程序不支持，或者无法进行更改...
    if (!bFlag){
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo SetPrinter [%d] fails"), GetLastError());
        GlobalFree(pi2);
        ClosePrinter(hPrinter);
        if (pDevMode){
			GlobalFree(pDevMode);
		}    
        return FALSE;
    }

	//Tell other apps that there was a change...
	//告诉其他应用有变化...
    SendMessageTimeout(HWND_BROADCAST, WM_DEVMODECHANGE, 0L, (LPARAM)(LPCSTR)pszPrinterName, SMTO_NORMAL, 1000, NULL);

    // Clean up...
	if (pi2) {
		GlobalFree(pi2);
	}
	if (hPrinter) {
		ClosePrinter(hPrinter);
	}
	if (pDevMode) {
		GlobalFree(pDevMode);
	}

	return TRUE;
}

/******************************************************************************************
Name	            :	SetPrintSettingInfo
Date	            :	2020-05-18
Description         :	设置打印机设置
Input parameters    :   hPrinter        打印机句柄
						pszPrinterName  打印机名称指针
						pszPrintSetting 设置打印机设置JSON
							paperSize	设置打印纸张(设置值必须是打印机直接的纸型)
								8   A3 297 x 420 mm
								9   A4 210 x 297 mm
								256 Custom
							pageWidth	设置打印页面的宽,单位为毫米 如果paperSize不为256，该值将忽略
							pageHeight	设置打印页面的高,单位为毫米 如果paperSize不为256，该值将忽略
							orientation	设置打印方向，横向或者纵向 1为纵向，2为横向
							copies	    设置打印的份数
							paperSource 设置纸张来源
******************************************************************************************/
BOOL SetPrintSettingInfoEx(TCHAR* pszPrinterName, TCHAR* pszPrintSetting) {
	std::string jsonStr;
	WcharToString(jsonStr, pszPrintSetting);

	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(jsonStr, root))
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo json parse [%s] fails"), pszPrintSetting);
		return FALSE;
	}

	DWORD dwSize = 0;
	// Open printer
	HANDLE hPrinter;
	if (!::OpenPrinter(pszPrinterName, &hPrinter, NULL)) {
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetPrintSettingInfo OpenPrinter [%s] fails"), pszPrinterName);
		return FALSE;
	}

	LONG lngPrinter = ::DocumentProperties(0, hPrinter, pszPrinterName, NULL, NULL, 0);
	if (lngPrinter < 0)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo DocumentProperties1 [%s] fails"), pszPrinterName);
		return FALSE;
	}
	DWORD    dwRet = 0;
	DEVMODE* pDevMode = (DEVMODE*)::GlobalAlloc(GPTR, lngPrinter);
	dwRet = ::DocumentProperties(0, hPrinter, pszPrinterName, pDevMode, NULL, DM_OUT_BUFFER);
	if (dwRet != IDOK)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetPrintSettingInfo DocumentProperties2 [%s] fails"), pszPrinterName);
		if (hPrinter) {
			::ClosePrinter(hPrinter);
		}
		if (pDevMode) {
			::GlobalFree(pDevMode);
		}
		return FALSE;
	}
	pDevMode->dmPaperSize = root["paperSize"].asInt();
	pDevMode->dmFields = pDevMode->dmFields | pDevMode->dmPaperSize;
	if (pDevMode->dmPaperSize == DMPAPER_USER) {
		pDevMode->dmPaperLength = root["paperHeight"].asInt();
		pDevMode->dmFields = pDevMode->dmFields | pDevMode->dmPaperLength;
		pDevMode->dmPaperWidth = root["paperWidth"].asInt();
		pDevMode->dmFields = pDevMode->dmFields | pDevMode->dmPaperWidth;
	}
	pDevMode->dmOrientation = root["orientation"].asInt();
	pDevMode->dmFields = pDevMode->dmFields | pDevMode->dmOrientation;
	pDevMode->dmCopies = root["copies"].asInt();
	pDevMode->dmFields = pDevMode->dmFields | pDevMode->dmCopies;
	//pDevMode->dmDefaultSource = root["paperSource"].asInt();
	dwRet = ::DocumentProperties(0, hPrinter, pszPrinterName, pDevMode, pDevMode, DM_OUT_BUFFER | DM_IN_BUFFER);
	if (dwRet != IDOK)
	{
		DWORD errCode = GetLastError();
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetPrintSettingInfo DocumentProperties3 [%s] fails : %d"), pszPrinterName, errCode);
		if (hPrinter) {
			::ClosePrinter(hPrinter);
		}
		if (pDevMode) {
			::GlobalFree(pDevMode);
		}
		return FALSE;
	}
	if (hPrinter) {
		::ClosePrinter(hPrinter);
	}
	if (pDevMode) {
		::GlobalFree(pDevMode);
	}
	return TRUE;
}