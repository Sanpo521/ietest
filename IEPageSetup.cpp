#include <atlbase.h>
#include <json/json.h>
#include "DebugInfoPrint.h"
#include "IEPageSetup.h"
#include "StringUtil.h"
#include "PrintUtil.h"
#include "UnitConversionUtil.h"

/******************************************************************************************
Name	            :	GetIEPageSetup
Date	            :	2020-05-23
Description         :	获取IE打印页面配置
Input parameters    :
Output parameters   :   pPageSetup IE打印页面配置指针 json串
							footer\header特殊配置说明：
							&b:空 
							&u:URL 
							&p:页码 
							&P:总页数 
							&w:标题 
							&d:短格式的日期 
							&D:长格式的日期 
							&t:时间 
							&T:采用24小时制的时间
							marginTop\marginLeft\marginRight\marginBottom单位为毫米
******************************************************************************************/
BOOL GetIEPageSetup(TCHAR* pPageSetup) {
	//定义有关的 hKEY, 在查询结束时要关闭。 
	HKEY hKEY; 
	LPCTSTR iePageSetup = _T("Software\\Microsoft\\Internet Explorer\\PageSetup");

	long ret = ::RegOpenKeyEx(HKEY_CURRENT_USER, iePageSetup, 0, KEY_READ, &hKEY);
	//如果无法打开hKEY，则终止程序的执行 
	if (ret != ERROR_SUCCESS) 
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegOpenKeyEx [%d] fails"), ret);
		return FALSE;
	}
	Json::Value jsonRoot; //定义根节点
	TCHAR lpwData[MAX_PATH] = { 0 };
	//dwType表示查询数据的类型
	DWORD dwType = REG_SZ; 
	//dwSize表示预设置的数据长度。 
	DWORD dwSize;
	std::string jsonStr;
	ret = ::RegQueryValueEx(hKEY, _T("footer"), NULL, &dwType, (LPBYTE)lpwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegQueryValueEx footer [%d] fails"), ret);
		return FALSE;
	}
	WcharToString(jsonStr, lpwData);
	jsonRoot["footer"] = Json::Value(jsonStr);
	dwSize = MAX_PATH;
	ret = ::RegQueryValueEx(hKEY, _T("header"), NULL, &dwType, (LPBYTE)lpwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegQueryValueEx header [%d] fails"), ret);
		return FALSE;
	}
	WcharToString(jsonStr, lpwData);
	jsonRoot["header"] = Json::Value(jsonStr);
	//注册表里的单位是英寸，打印设置中是毫米，1英寸=25.4毫米
	dwSize = MAX_PATH;
	ret = ::RegQueryValueEx(hKEY, _T("margin_bottom"), NULL, &dwType, (LPBYTE)lpwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegQueryValueEx margin_bottom [%d] fails"), ret);
		return FALSE;
	}

	float mb = round(Inch2Millimeter(lpwData));
	//WcharToString(jsonStr, lpwData);
	jsonRoot["marginBottom"] = Json::Value(mb);
	dwSize = MAX_PATH;
	ret = ::RegQueryValueEx(hKEY, _T("margin_left"), NULL, &dwType, (LPBYTE)lpwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegQueryValueEx margin_left [%d] fails"), ret);
		return FALSE;
	}
	float ml = round(Inch2Millimeter(lpwData));
	//WcharToString(jsonStr, lpwData);
	jsonRoot["marginLeft"] = Json::Value(ml);
	dwSize = MAX_PATH;
	ret = ::RegQueryValueEx(hKEY, _T("margin_right"), NULL, &dwType, (LPBYTE)lpwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegQueryValueEx margin_right [%d] fails"), ret);
		return FALSE;
	}
	float mr = round(Inch2Millimeter(lpwData));
	//WcharToString(jsonStr, lpwData);
	jsonRoot["marginRight"] = Json::Value(mr);
	dwSize = MAX_PATH;
	ret = ::RegQueryValueEx(hKEY, _T("margin_top"), NULL, &dwType, (LPBYTE)lpwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] GetIEPageSetup RegQueryValueEx margin_top [%d] fails"), ret);
		return FALSE;
	}
	float mt = round(Inch2Millimeter(lpwData));
	//WcharToString(jsonStr, lpwData);
	jsonRoot["marginTop"] = Json::Value(mt);
	std::wstring wszDest;
	StringToWstring(wszDest, jsonRoot.toStyledString());
	const wchar_t* pwidstr = wszDest.c_str();
	_tcscpy_s(pPageSetup, MAX_JSON, pwidstr);
	// 程序结束前要关闭已经打开的 hKEY。 
	::RegCloseKey(hKEY);

	return TRUE;
}

/******************************************************************************************
Name	            :	SetIEPageSetup
Date	            :	2020-05-24
Description         :	设置IE打印页面配置
Input parameters    :   pPageSetup IE打印页面配置指针 json串
							footer\header特殊配置说明：
							&b:空
							&u:URL
							&p:页码
							&P:总页数
							&w:标题
							&d:短格式的日期
							&D:长格式的日期
							&t:时间
							&T:采用24小时制的时间
							marginTop\marginLeft\marginRight\marginBottom单位为毫米
Output parameters   :
******************************************************************************************/
BOOL SetIEPageSetup(TCHAR* pPageSetup) {
	std::string jsonStr;
	WcharToString(jsonStr, pPageSetup);

	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(jsonStr, root))
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup json parse [%s] fails"), pPageSetup);
		return FALSE;
	}
	//定义有关的 hKEY, 在查询结束时要关闭。 
	HKEY hKEY;
	LPCTSTR iePageSetup = _T("Software\\Microsoft\\Internet Explorer\\PageSetup");

	long ret = ::RegOpenKeyEx(HKEY_CURRENT_USER, iePageSetup, 0, KEY_READ | KEY_WRITE, &hKEY);
	//如果无法打开hKEY，则终止程序的执行 
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegOpenKeyEx [%d] fails"), ret);
		return FALSE;
	}
	//dwSize表示预设置的数据长度。 
	DWORD dwSize;
	std::wstring wszDest;
	const wchar_t* pwidstr;
	TCHAR millimeter[MAX_IEPS], inch[MAX_IEPS];
	if (!root["footer"].isNull()) {
		StringToWstring(wszDest, root["footer"].asCString());
		pwidstr = wszDest.c_str();
		ret = ::RegSetValueEx(hKEY, _T("footer"), NULL, REG_SZ, (CONST BYTE*)pwidstr, (_tcslen(pwidstr) + 1) * sizeof(TCHAR));
		if (ret != ERROR_SUCCESS)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegSetValueEx footer [%d] fails"), GetLastError());
			return FALSE;
		}
	}
	if (!root["header"].isNull()) {
		StringToWstring(wszDest, root["header"].asCString());
		pwidstr = wszDest.c_str();
		ret = ::RegSetValueEx(hKEY, _T("header"), NULL, REG_SZ, (CONST BYTE*)pwidstr, (_tcslen(pwidstr) + 1) * sizeof(TCHAR));
		if (ret != ERROR_SUCCESS)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegSetValueEx header [%d] fails"), ret);
			return FALSE;
		}
	}
	//注册表里的单位是英寸，打印设置中是毫米，1英寸=25.4毫米
	if (!root["marginBottom"].isNull()) {
		StringToWstring(wszDest, root["marginBottom"].asCString());
		//pwidstr = wszDest.c_str();
		_stprintf_s(millimeter, MAX_IEPS, _T("%s"), wszDest.c_str());
		Millimeter2Inch(millimeter, inch);
		pwidstr = inch;
		ret = ::RegSetValueEx(hKEY, _T("margin_bottom"), NULL, REG_SZ, (CONST BYTE*)pwidstr, (_tcslen(pwidstr) + 1) * sizeof(TCHAR));
		if (ret != ERROR_SUCCESS)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegSetValueEx margin_bottom [%d] fails"), ret);
			return FALSE;
		}
	}
	if (!root["marginLeft"].isNull()) {
		StringToWstring(wszDest, root["marginLeft"].asCString());
		//pwidstr = wszDest.c_str();
		_stprintf_s(millimeter, MAX_IEPS, _T("%s"), wszDest.c_str());
		Millimeter2Inch(millimeter, inch);
		pwidstr = inch;
		Millimeter2Inch((TCHAR*)wszDest.c_str(), (TCHAR*)pwidstr);
		ret = ::RegSetValueEx(hKEY, _T("margin_left"), NULL, REG_SZ, (CONST BYTE*)pwidstr, (_tcslen(pwidstr) + 1) * sizeof(TCHAR));
		if (ret != ERROR_SUCCESS)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegSetValueEx margin_left [%d] fails"), ret);
			return FALSE;
		}
	}
	if (!root["marginRight"].isNull()) {
		StringToWstring(wszDest, root["marginRight"].asCString());
		//pwidstr = wszDest.c_str();
		_stprintf_s(millimeter, MAX_IEPS, _T("%s"), wszDest.c_str());
		Millimeter2Inch(millimeter, inch);
		pwidstr = inch;
		Millimeter2Inch((TCHAR*)wszDest.c_str(), (TCHAR*)pwidstr);
		ret = ::RegSetValueEx(hKEY, _T("margin_right"), NULL, REG_SZ, (CONST BYTE*)pwidstr, (_tcslen(pwidstr) + 1) * sizeof(TCHAR));
		if (ret != ERROR_SUCCESS)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegSetValueEx margin_right [%d] fails"), ret);
			return FALSE;
		}
	}
	if (!root["marginTop"].isNull()) {
		StringToWstring(wszDest, root["marginTop"].asCString());
		//pwidstr = wszDest.c_str();
		_stprintf_s(millimeter, MAX_IEPS, _T("%s"), wszDest.c_str());
		Millimeter2Inch(millimeter, inch);
		pwidstr = inch;
		Millimeter2Inch((TCHAR*)wszDest.c_str(), (TCHAR*)pwidstr);
		ret = ::RegSetValueEx(hKEY, _T("margin_top"), NULL, REG_SZ, (CONST BYTE*)pwidstr, (_tcslen(pwidstr) + 1) * sizeof(TCHAR));
		if (ret != ERROR_SUCCESS)
		{
			OutputDebugPrintf(_T("[SanpoWebPrintWS] SetIEPageSetup RegSetValueEx margin_top [%d] fails"), ret);
			return FALSE;
		}
	}


	// 程序结束前要关闭已经打开的 hKEY。 
	::RegCloseKey(hKEY);

	return TRUE;
}