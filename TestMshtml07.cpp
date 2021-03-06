#include <atlbase.h>
#include <atlcom.h>
#include <string>
CComModule _Module;
#include <atlwin.h>
#include <atlstr.h>
#include "Printer.h"
#include <thread>
#include "PrintUtil.h"
#include "IEPageSetup.h"
#include "CreateTempFile.h"

#pragma comment(lib,"atls")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"urlmon.lib")
//#import "C:\\windows\\system32\\mshtml.tlb" // location of mshtml.tlb

#define MAX_LOADSTRING			100
#define MAX_URL					1000

#define IDB_GOURL				3301
#define IDB_PRINT				3302
#define IDB_PRINTPREVIEW		3303
#define IWB_DOCUMENTCOMPLETE	3304

#define MAX_PRINTTEMPLATE_PARAMS					3000

TCHAR pOptions[MAX_PATH];
TCHAR pTemplateParams[MAX_PRINTTEMPLATE_PARAMS];

CString m_ContentHTML;

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
TCHAR szSystemPath[MAX_PATH];
TCHAR buf[MAX_URL];

DWORD ret;
HINSTANCE hInstWindow;
HWND hWndWindow, hWndWebBrower, hWndEditUrl, hWndBtnGo, hWndBtnPrint, hWndBtnPrintPreview;
HWND hWndEditPrinterSetOld, hWndEditPrinterSetNew, hWndEditPageSetupOld, hWndEditPageSetupNew;
TCHAR* n_str;
size_t len;

IWebBrowser2* iWebBrowser;
VARIANT varMyURL;
BOOL bolLoad = FALSE;

HRESULT  hr;

BEGIN_OBJECT_MAP(ObjectMap)

END_OBJECT_MAP()

LPDEVMODE GetLandscapeDevMode(HWND hWnd, TCHAR* pDevice) {
	HANDLE      hPrinter;
	LPDEVMODE   pDevMode;
	DWORD       dwNeeded, dwRet;

	/* Start by opening the printer */
	if (!OpenPrinter(pDevice, &hPrinter, NULL))
		return NULL;

	/*
	 * Step 1:
	 * Allocate a buffer of the correct size.
	 */
	dwNeeded = DocumentProperties(hWnd,
		hPrinter,       /* Handle to our printer. */
		pDevice,        /* Name of the printer. */
		NULL,           /* Asking for size, so */
		NULL,           /* these are not used. */
		0);             /* Zero returns buffer size. */
	pDevMode = (LPDEVMODE)malloc(dwNeeded);

	/*
	 * Step 2:
	 * Get the default DevMode for the printer and
	 * modify it for your needs.
	 */
	dwRet = DocumentProperties(hWnd,
		hPrinter,
		pDevice,
		pDevMode,       /* The address of the buffer to fill. */
		NULL,           /* Not using the input buffer. */
		DM_OUT_BUFFER); /* Have the output buffer filled. */
	if (dwRet != IDOK)
	{
		/* If failure, cleanup and return failure. */
		free(pDevMode);
		ClosePrinter(hPrinter);
		return NULL;
	}

	/*
		 * Make changes to the DevMode which are supported.
	 */
	if (pDevMode->dmFields & DM_ORIENTATION)
	{
		/* If the printer supports paper orientation, set it.*/
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	}

	if (pDevMode->dmFields & DM_DUPLEX)
	{
		/* If it supports duplex printing, use it. */
		pDevMode->dmDuplex = DMDUP_HORIZONTAL;
	}

	/*
	 * Step 3:
	 * Merge the new settings with the old.
	 * This gives the driver an opportunity to update any private
	 * portions of the DevMode structure.
	 */
	dwRet = DocumentProperties(hWnd,
		hPrinter,
		pDevice,
		pDevMode,       /* Reuse our buffer for output. */
		pDevMode,       /* Pass the driver our changes. */
		DM_IN_BUFFER |  /* Commands to Merge our changes and */
		DM_OUT_BUFFER); /* write the result. */

	/* Finished with the printer */
	ClosePrinter(hPrinter);

	if (dwRet != IDOK)
	{
		/* If failure, cleanup and return failure. */
		free(pDevMode);
		return NULL;
	}

	/* Return the modified DevMode structure. */

	return pDevMode;
}

VOID GotoUrl(IWebBrowser2* iWebBrowser, TCHAR* bufUrl) {
	VariantInit(&varMyURL);
	varMyURL.vt = VT_BSTR;
	varMyURL.bstrVal = SysAllocString(bufUrl);
	iWebBrowser->Navigate2(&varMyURL, 0, 0, 0, 0);
	VariantClear(&varMyURL);
	iWebBrowser->Release();
}

VOID SaveToFile(IHTMLDocument2* _pHtmlDoc2, IHTMLDocument3* _pHtmlDoc3, IHTMLDocument5* _pHtmlDoc5, TCHAR* pFileName) {
	IHTMLElementCollection* _pBaseElementCollection;
	IHTMLElement* _pHtmlElement;
	IHTMLBaseElement* _pBaseElement;
	LONG length;
	CComBSTR strBase("base");
	CComBSTR strHead("head");
	hr = _pHtmlDoc3->getElementsByTagName(strBase, &_pBaseElementCollection);
	if (hr != S_OK) {
		MessageBox(NULL, _T("IHTMLDocument3::getElementsByTagName 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
	}
	hr = _pBaseElementCollection->get_length(&length);
	if (hr != S_OK) {
		MessageBox(NULL, _T("IHTMLElementCollection::get_length 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
	}
	BSTR locationURL = NULL;
	iWebBrowser->get_LocationURL(&locationURL);
	//判断是否存在根元素
	if (length == 0) {
		//取得IHTMLElement类型根元素
		IHTMLElement* _pBaseElementT;
		hr = _pHtmlDoc2->createElement(strBase, &_pBaseElementT);
		if (hr != S_OK) {
			MessageBox(NULL, _T("IHTMLDocument2::createElement 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		//将IHTMLElement类型根元素 转化为 IHTMLBaseElement类型
		hr = _pBaseElementT->QueryInterface(IID_IHTMLBaseElement, (void**)&_pBaseElement);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLBaseElement 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		//设置IHTMLBaseElement类型属性
		hr = _pBaseElement->put_href(locationURL);
		if (hr != S_OK) {
			MessageBox(NULL, _T("IHTMLBaseElement::put_href 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		//设置内容中头元素IHTMLElementCollection类型值
		IHTMLElementCollection* _pHeadElementCollection = NULL;
		hr = _pHtmlDoc3->getElementsByTagName(strHead, &_pHeadElementCollection);
		VARIANT varIndex;
		V_VT(&varIndex) = VT_I4;
		V_I4(&varIndex) = 0;
		IDispatch* dispItem = NULL;
		//取得第一个元素
		hr = _pHeadElementCollection->item(varIndex, varIndex, &dispItem);
		//将IHTMLElement类型根元素 转化为 _pHtmlElement2类型
		IHTMLElement2* _pHtmlElement2;
		hr = dispItem->QueryInterface(IID_IHTMLElement2, (void**)&_pHtmlElement2);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLElement2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		//插入头元素前插入根元素
		IHTMLElement* _pInsertBaseElement = NULL;
		hr = _pHtmlElement2->insertAdjacentElement(SysAllocString(_T("afterBegin")), (IHTMLElement*)_pBaseElement, &_pInsertBaseElement);
		VariantClear(&varIndex);
		_pBaseElementT->Release();
		_pHtmlElement2->Release();
		dispItem->Release();
		_pHeadElementCollection->Release();
		if (NULL != _pInsertBaseElement)
			_pInsertBaseElement->Release();
	}
	else {
		VARIANT varIndex;
		V_VT(&varIndex) = VT_I4;
		V_I4(&varIndex) = 0;
		IDispatch* dispItem;
		_pBaseElementCollection->item(varIndex, varIndex, &dispItem);
		//将IHTMLElement类型根元素 转化为 IHTMLBaseElement类型
		hr = dispItem->QueryInterface(IID_IHTMLBaseElement, (void**)&_pBaseElement);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLBaseElement 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		BSTR tlocationURL;
		_pBaseElement->get_href(&tlocationURL);
		if (NULL != tlocationURL) {
			if (lstrlen(tlocationURL)==0) {
				_pBaseElement->put_href(locationURL);
			}
		}
		VariantClear(&varIndex);
		dispItem->Release();
	}
	_pBaseElement->Release();

	IHTMLElementCollection* _pElementCollection;
	_pHtmlDoc3->getElementsByTagName(SysAllocString(_T("HTML")), &_pElementCollection);
	_pBaseElementCollection->get_length(&length);
	if (length > 0) {
		VARIANT varIndex;
		V_VT(&varIndex) = VT_I4;
		V_I4(&varIndex) = 0;
		IDispatch* dispItem;
		_pElementCollection->item(varIndex, varIndex, &dispItem);
		VARIANT varDisplayUrl;
		V_VT(&varDisplayUrl) = VT_BSTR;
		V_BSTR(&varDisplayUrl) = locationURL;
		//将IHTMLElement类型根元素 转化为 IHTMLBaseElement类型
		hr = dispItem->QueryInterface(IID_IHTMLElement, (void**)&_pHtmlElement);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLBaseElement 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		//__IE_DisplayUrl			
		//Retrieves a string that specifies theURLof the document.
		//检索一个字符串，该字符串指定文档的URL。
		_pHtmlElement->setAttribute(SysAllocString(_T("__IE_DisplayURL")), varDisplayUrl, 0);
		VariantClear(&varIndex);
		VariantClear(&varDisplayUrl);
		dispItem->Release();
	}
	_pElementCollection->Release();

	BSTR compatMode;
	hr = _pHtmlDoc5->get_compatMode(&compatMode);
	if (hr != S_OK) {
		MessageBox(NULL, _T("IHTMLDocument3 get_documentElement 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
	}

	HANDLE hTempFile = INVALID_HANDLE_VALUE;
	GetTempHtmlFileName(pFileName);
	
	if (_tcsicmp(_T("CSS1Compat"), compatMode) == 0) {
		TCHAR strFirst[MAX_PATH];
		_stprintf_s(strFirst, MAX_PATH, _T(""));
		hTempFile = CreateTempHtmlFile(pFileName);
		WriteTempHtmlFile(hTempFile, strFirst);
	}
	_pHtmlDoc3->get_documentElement(&_pHtmlElement);
	BSTR outerHtml;
	_pHtmlElement->get_outerHTML(&outerHtml);

	hTempFile = CreateTempHtmlFile(pFileName);
	WriteTempHtmlFile(hTempFile, outerHtml);

	_pHtmlElement->Release();
	_pBaseElementCollection->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//*********************
	RECT rc;
	static CAxWindow WinContainer;

	CString strFunction = _T("shell.Explorer.2");
	BSTR b = strFunction.AllocSysString();
	LPOLESTR pszName = b;

	GetClientRect(hWnd, &rc);

	GUID guid;
	//**********************
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (LOWORD(wParam))
		{
		case IDB_GOURL: {
			TCHAR oldIePaggeSetup[MAX_URL];
			if (GetIEPageSetup(oldIePaggeSetup)) {
				SetWindowText(hWndEditPageSetupOld, oldIePaggeSetup);
			}else {
				MessageBox(NULL, _T("获取IE页面配置失败"), _T("系统提示"), MB_OK);
			}
			TCHAR newIePaggeSetup[MAX_URL];
			GetWindowText(hWndEditPageSetupNew, newIePaggeSetup, MAX_URL);
			if (!SetIEPageSetup(newIePaggeSetup)) {
				MessageBox(NULL, _T("设置IE页面配置失败"), _T("系统提示"), MB_OK);
			}
			TCHAR printerName[MAX_URL];
			if (!GetDefaultPrinterInfo(printerName)) {
				MessageBox(NULL, _T("没有设置默认打印机"), _T("系统提示"), MB_OK);
			}
			else {
				TCHAR printerSettingA[MAX_JSON];
				if (GetPrintSettingInfo(printerName, printerSettingA)) {
					SetWindowText(hWndEditPrinterSetOld, printerSettingA);
				}
				else {
					MessageBox(NULL, _T("获取打印机设置失败"), _T("系统提示"), MB_OK);
				}
				TCHAR printerSetting[MAX_JSON];
				GetWindowText(hWndEditPrinterSetNew, printerSetting, MAX_JSON);
				if (!SetPrintSettingInfo(printerName, printerSetting)) {
					MessageBox(NULL, _T("设置打印机设置失败"), _T("系统提示"), MB_OK);
				}
			}
			TCHAR bufUrl[MAX_URL];
			GetWindowText(hWndEditUrl, bufUrl, MAX_URL);
			GotoUrl(iWebBrowser, bufUrl);
			bolLoad = TRUE;
			break;
		}
		case IDB_PRINT:
			GetWindowText(hWndEditUrl, buf, MAX_URL);
			ret = GetSystemDirectory(szSystemPath, MAX_PATH);
			if (!ret) {
				printf("GetSystemDirectory fail (%ld)\n", GetLastError());
				break;
			}
			len = (sizeof(szSystemPath) + sizeof(_T("\\mshtml.dll,PrintHTML \"")) + sizeof(buf) + sizeof(_T("\""))) / sizeof(TCHAR);
			n_str = new TCHAR[len + 1];
			_tcscpy_s(n_str, len, szSystemPath);
			_tcscat_s(n_str, len, _T("\\mshtml.dll,PrintHTML \""));
			_tcscat_s(n_str, len, buf);
			_tcscat_s(n_str, len, _T("\""));
			ShellExecute(0, NULL, _T("rundll32.exe"), n_str, NULL, SW_NORMAL);
			break;
		case IDB_PRINTPREVIEW:
		{
			GetWindowText(hWndEditUrl, buf, MAX_URL);
			ret = GetSystemDirectory(szSystemPath, MAX_PATH);
			if (!ret) {
				printf("GetSystemDirectory fail (%ld)\n", GetLastError());
				break;
			}
			len = (sizeof(szSystemPath) + sizeof(_T("\\mshtml.dll,ShowHTMLDialog \"res://ietest.exe/PT.HTM\""))) / sizeof(TCHAR);
			n_str = new TCHAR[len + 1];
			_tcscpy_s(n_str, len, szSystemPath);
			_tcscat_s(n_str, len, _T("\\mshtml.dll,ShowHTMLDialog \"res://ietest.exe/PT.HTM\""));
			ShellExecute(0, NULL, _T("rundll32.exe"), n_str, NULL, SW_NORMAL);
		}
		break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);
		//第一次启动对话框时的大小变化不做处理
		if (0 == rc.left && 0 == rc.right && 0 == rc.top && 0 == rc.bottom)
		{
		}
		else
		{
			//如果是按下了最小化，则触发条件，这时不保存对话框数据
			if (0 != rc.right && 0 != rc.bottom)
			{
				//重绘函数，用以更新对话框上控件的位置和大小
				MoveWindow(hWndWebBrower, rc.left, rc.top, rc.right, rc.bottom - 130, true);
				MoveWindow(hWndEditPageSetupNew, 10, rc.bottom - 120, (rc.right - 30) / 2, 30, true);
				MoveWindow(hWndEditPageSetupOld, (rc.right - 30) / 2 + 20, rc.bottom - 120, (rc.right - 30) / 2, 30, true);
				MoveWindow(hWndEditPrinterSetNew, 10, rc.bottom - 80, (rc.right - 30) / 2, 30, true);
				MoveWindow(hWndEditPrinterSetOld, (rc.right-30)/2+20, rc.bottom - 80, (rc.right - 30) / 2, 30, true);
				MoveWindow(hWndEditUrl, 10, rc.bottom - 40, 800, 30, true);
				MoveWindow(hWndBtnGo, 820, rc.bottom - 40, 120, 30, true);
				MoveWindow(hWndBtnPrint, 950, rc.bottom - 40, 120, 30, true);
				MoveWindow(hWndBtnPrintPreview, 1080, rc.bottom - 40, 120, 30, true);
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		_Module.Init(ObjectMap, hInst, &guid);
		hWndWebBrower = WinContainer.Create(hWnd, rc, 0, WS_CHILD | WS_VISIBLE);
		WinContainer.CreateControl(pszName);
		WinContainer.QueryControl(__uuidof(IWebBrowser2), (void**)&iWebBrowser);
		hWndEditPageSetupNew = CreateWindow(_T("edit"), _T("{\"footer\":\"&b\",\"header\":\"&b\",\"marginBottom\":\"21\",\"marginLeft\":\"15\",\"marginRight\":\"15\",\"marginTop\":\"21\"}"), WS_CHILD | WS_VISIBLE | WS_BORDER, 10, rc.bottom - 120, (rc.right - 30) / 2, 30, hWnd, NULL, NULL, NULL);
		hWndEditPageSetupOld = CreateWindow(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, (rc.right - 30) / 2 + 20, rc.bottom - 120, (rc.right - 30) / 2, 30, hWnd, NULL, NULL, NULL);
		hWndEditPrinterSetNew = CreateWindow(_T("edit"), _T("{\"copies\":2,\"orientation\":2,\"paperHeight\":4200,\"paperSize\":8,\"paperSource\":15,\"paperWidth\":2970}"), WS_CHILD | WS_VISIBLE | WS_BORDER, 10, rc.bottom - 80, (rc.right - 30) / 2, 30, hWnd, NULL, NULL, NULL);
		hWndEditPrinterSetOld = CreateWindow(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, (rc.right - 30) / 2 + 20, rc.bottom - 80, (rc.right - 30) / 2, 30, hWnd, NULL, NULL, NULL);
		//file:///F:/WorkDocument/hntp/插件/topprint/topprint.html
		//file:///F:/WorkSpace/visualStudio/vc/ietest-2004091630/resouce/pagesource.html
		hWndEditUrl = CreateWindow(_T("edit"), _T("file:///F:/WorkDocument/hntp/插件/topprint/topprinth.html"), WS_CHILD | WS_VISIBLE | WS_BORDER, 10, rc.bottom - 40, 800, 30, hWnd, NULL, NULL, NULL);
		hWndBtnGo = CreateWindow(_T("Button"), _T("访问"), WS_VISIBLE | WS_CHILD, 820, rc.bottom - 40, 120, 30, hWnd, (HMENU)IDB_GOURL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hWndBtnPrint = CreateWindow(_T("Button"), _T("打印"), WS_VISIBLE | WS_CHILD, 950, rc.bottom - 40, 120, 30, hWnd, (HMENU)IDB_PRINT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hWndBtnPrintPreview = CreateWindow(_T("Button"), _T("打印预览"), WS_VISIBLE | WS_CHILD, 1080, rc.bottom - 40, 120, 30, hWnd, (HMENU)IDB_PRINTPREVIEW, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case IWB_DOCUMENTCOMPLETE:
	{
		IDispatch* pDp = NULL;
		hr = iWebBrowser->get_Document(&pDp);
		if (hr != S_OK) {
			MessageBox(NULL, _T("DWebBrowser2::get_Document 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		IHTMLDocument2* _pHtmlDoc2;
		hr = pDp->QueryInterface(IID_IHTMLDocument2, (void**)&_pHtmlDoc2);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLDocument2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		IHTMLDocument3* _pHtmlDoc3;
		hr = pDp->QueryInterface(IID_IHTMLDocument3, (void**)&_pHtmlDoc3);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLDocument3 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		IHTMLDocument4* _pHtmlDoc4;
		hr = pDp->QueryInterface(IID_IHTMLDocument4, (void**)&_pHtmlDoc4);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLDocument4 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		IHTMLDocument5* _pHtmlDoc5;
		hr = pDp->QueryInterface(IID_IHTMLDocument5, (void**)&_pHtmlDoc5);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLDocument5 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}

		TCHAR FileName[MAX_PATH];
		SaveToFile(_pHtmlDoc2, _pHtmlDoc3, _pHtmlDoc5, FileName);
		TCHAR ShortFileName[MAX_PATH];
		GetShortPathName(FileName, ShortFileName, MAX_PATH);


		IHTMLEventObj* _pHtmlEvent;
		VARIANT vName;
		hr = _pHtmlDoc4->createEventObject(&vName, &_pHtmlEvent);
		//hr = _pHtmlWnd2->get_event(&_pHtmlEvent);
		if (hr != S_OK) {
			MessageBox(NULL, _T("IHTMLWindow2::get_event 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		IHTMLEventObj2* _pHtmlEvent2;
		hr = _pHtmlEvent->QueryInterface(IID_IHTMLEventObj2, (void**)&_pHtmlEvent2);
		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLEventObj2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		VARIANT cvarTemp;
		SAFEARRAY* sfArray;
		// Creates a new one-dimensional array
		//sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);

		//__IE_TemplateUrl(VT_EMPTY)
		//	__IE_ParentHWND(VT_UINT)
		//	__IE_HeaderString(VT_BSTR)
		//	__IE_FooterString(VT_BSTR)
		//	__IE_OutlookHeader(VT_UNKNOWN)
		//	__IE_BaseLineScale(VT_INT)
		//	__IE_uPrintFlags(VT_UINT)
		//	__IE_ContentDocumentUrl(VT_BSTR)
		//	__IE_ContentSelectionUrl(VT_BSTR)
		//	__IE_PrinterCMD_Printer(VT_BSTR)
		//	__IE_PrinterCMD_Device(VT_BSTR)
		//	__IE_PrinterCMD_Port(VT_BSTR)
		//	__IE_BrowseDocument(VT_UNKNOWN)
		//	__IE_TemporaryFiles(VT_ARRAY)
		//	__IE_PrinterCMD_DevNames(VT_I4)
		//	__IE_PrinterCMD_DevMode(VT_I4)
		//	__IE_PrintType(VT_BSTR)

		//res://ieframe.dll/preview.dlg


		//__IE_BaseLineScale		
		//Retrieves an integer specifying the font size of the print template.
		//检索一个整数，该整数指定打印模板的字体大小。
		//---------------------------------

		////__IE_BrowseDocument		
		////Retrieves thedocumentobject of the page being printed or print-previewed.	
		////检索正在打印或已打印预览的页面的文档对象。
		////---------------------------------
		//VARIANT vtUnkown;
		//V_VT(&vtUnkown) = VT_UNKNOWN;
		//V_UNKNOWN(&vtUnkown) = pDp;
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_BrowseDocument")), vtUnkown, 0);
		//VariantClear(&vtUnkown);

		//__IE_ContentDocumentUrl		
		//Retrieves a string specifying theURLof a temporary copy of the source document for the print template.	
		//检索一个字符串，该字符串指定打印模板的源文档的临时副本的URL。
		//---------------------------------
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = SysAllocString(FileName);
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_ContentDocumentUrl")), cvarTemp, 0);
		VariantClear(&cvarTemp);

		////__IE_ContentSelectionUrl	
		////Retrieves a string specifying theURLof a temporary .htm file containing the current selection in the browser.
		////检索一个字符串，该字符串指定浏览器中包含当前选择的临时.htm文件的URL。
		////---------------------------------
		//V_VT(&cvarTemp) = VT_BSTR;
		//V_BSTR(&cvarTemp) = SysAllocString(_T(""));
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_ContentSelectionUrl")), cvarTemp, 0);
		//VariantClear(&cvarTemp);

		////__IE_HeaderString		
		////Retrieves a string specifying the header string from thePage Setupdialog box.
		////从“页面设置”对话框中检索一个用于指定标题字符串的字符串。
		////---------------------------------
		//V_VT(&cvarTemp) = VT_BSTR;
		//V_BSTR(&cvarTemp) = SysAllocString(_T(""));
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_HeaderString")), cvarTemp, 0);
		//VariantClear(&cvarTemp);

		////__IE_FooterString		
		////Retrieves a string specifying the footer string from thePage Setupdialog box.
		////从“页面设置”对话框中检索指定页脚字符串的字符串。
		////---------------------------------
		//V_VT(&cvarTemp) = VT_BSTR;
		//V_BSTR(&cvarTemp) = SysAllocString(_T(""));
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_FooterString")), cvarTemp, 0);
		//VariantClear(&cvarTemp);

		////__IE_ActiveFrame		
		////Retrieves the index of the active frame in the frames collection.
		////检索框架集合中活动框架的索引。
		////---------------------------------
		//V_VT(&cvarTemp) = VT_I4;
		//V_I4(&cvarTemp) = 0;
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_ActiveFrame")), cvarTemp, 0);
		//VariantClear(&cvarTemp);

		////__IE_OutlookHeader		
		////Retrieves a string specifying theMicrosoft Outlookheader string.
		////检索指定Microsoft Outlookheader字符串的字符串。
		////---------------------------------
		//V_VT(&cvarTemp) = VT_BSTR;
		//V_BSTR(&cvarTemp) = SysAllocString(_T(""));
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_OutlookHeader")), cvarTemp, 0);
		//VariantClear(&cvarTemp);

	//	pEventObj2.setAttribute('__IE_PrinterCMD_Device', FDeviceW, 0);
	//	pEventObj2.setAttribute('__IE_PrinterCMD_Port', FPortW, 0);
	//	pEventObj2.setAttribute('__IE_PrinterCMD_Printer', FDriverW, 0);
	//	pEventObj2.setAttribute('__IE_PrinterCmd_DevMode', FDevModeHandle, 0);
	//	pEventObj2.setAttribute('__IE_PrinterCmd_DevNames', FDevNamesHandle, 0);
		/*********************************************************************************/
		HANDLE hPrinter = NULL;
		TCHAR* szPrinter = NULL;
		TCHAR szDevice[MAX_PATH];
		TCHAR szPort[MAX_PATH];
		TCHAR szPrinterEx[MAX_PATH];
		//HGLOBAL phDevNames;
		//HGLOBAL phDevMode;
		TCHAR phDevNames[MAX_PATH];
		TCHAR phDevMode[MAX_PATH];
		DWORD dwSize = 0;

		if ((!::GetDefaultPrinter(NULL, &dwSize)) && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			szPrinter = new TCHAR[dwSize + 1];
			if (::GetDefaultPrinter(szPrinter, &dwSize))
			{
				GetPrinterDeviceEx(szPrinter, phDevNames, phDevMode, szDevice, szPort, szPrinterEx);
			}
		}
		/*****************************************自定义参数开始**********************************************/
		////__Custom_Parameter_copies 打印份数
		//V_VT(&cvarTemp) = VT_UI4;
		//V_UI4(&cvarTemp) = 1;
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__Custom_Parameter_copies")), cvarTemp, 0);
		//VariantClear(&cvarTemp);
		//__Custom_Parameter_selectedPages 选择打印页
		V_VT(&cvarTemp) = VT_BOOL;
		V_BOOL(&cvarTemp) = FALSE;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__Custom_Parameter_selectedPages")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		//__Custom_Parameter_currentPage 当前页
		V_VT(&cvarTemp) = VT_BOOL;
		V_BOOL(&cvarTemp) = FALSE;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__Custom_Parameter_currentPage")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		//__Custom_Parameter_pageFrom 打印从第几页
		V_VT(&cvarTemp) = VT_UI4;
		V_UI4(&cvarTemp) = 0;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__Custom_Parameter_pageFrom")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		//__Custom_Parameter_pageTo 打印到第几页
		V_VT(&cvarTemp) = VT_UI4;
		V_UI4(&cvarTemp) = 0;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__Custom_Parameter_pageTo")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		//__Custom_Parameter_zoomValue 显示比例
		V_VT(&cvarTemp) = VT_UI4;
		V_UI4(&cvarTemp) = 75;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__Custom_Parameter_zoomValue")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		/*****************************************自定义参数结束**********************************************/
		//	pEventObj2.setAttribute('__IE_PrinterCMD_Device', FDeviceW, 0);
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = SysAllocString(szDevice);
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrinterCMD_Device")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		//	pEventObj2.setAttribute('__IE_PrinterCMD_Port', FPortW, 0);
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = SysAllocString(szPort);
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrinterCMD_Port")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		//	pEventObj2.setAttribute('__IE_PrinterCMD_Printer', FDriverW, 0);
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = SysAllocString(szPrinterEx);
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrinterCMD_Printer")), cvarTemp, 0);
		VariantClear(&cvarTemp);

		////V_VT(&cvarTemp) = VT_BSTR;
		////V_BSTR(&cvarTemp) = SysAllocString(phDevMode);
		////_pHtmlEvent2->setAttribute(SysAllocString(_T("Sanpo_DevMode")), cvarTemp, 0);
		////VariantClear(&cvarTemp);
		//
		//TCHAR* str;
		//int i = (int)wcstol(phDevMode, &str, 16);//十六进制  
		//VARIANT  varDevNames, varDevMode;
		//VariantInit(&varDevMode);
		//V_VT(&varDevMode) = VT_UI8;
		//V_UI8(&varDevMode) = i;
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrinterCmd_DevMode")), varDevMode, 0);
		//VariantClear(&varDevMode);

		////V_VT(&cvarTemp) = VT_BSTR;
		////V_BSTR(&cvarTemp) = SysAllocString(phDevNames);
		////_pHtmlEvent2->setAttribute(SysAllocString(_T("Sanpo_DevNames")), cvarTemp, 0);
		////VariantClear(&cvarTemp);

		//VariantInit(&varDevNames);
		//i = (int)wcstol(phDevNames, &str, 16);//十六进制  
		//V_VT(&varDevNames) = VT_UI4;
		//V_UI8(&varDevNames) = i;
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrinterCmd_DevNames")), varDevNames, 0);
		//VariantClear(&varDevNames);

		delete[]szPrinter;
		/*********************************************************************************/

	//	if FPrint then
	//		pEventObj2.setAttribute('__IE_PrintType', 'NoPrompt', 0)
	//	else
	//		pEventObj2.setAttribute('__IE_PrintType', 'Preview', 0);
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = SysAllocString(_T("Preview"));
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrintType")), cvarTemp, 0);
		VariantClear(&cvarTemp);

		IMoniker* pURLMoniker;
		BSTR bstrURL = SysAllocString(_T("res://ietest.exe/PT.HTM"));
		if (!SUCCEEDED(::CreateURLMoniker(NULL, bstrURL, &pURLMoniker))) {
			MessageBox(NULL, TEXT("CreateURLMoniker Failed."), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONERROR);
			break;
		}
		////__IE_TemplateUrl		
		////Retrieves a string specifying theURLof the print template.
		////检索一个字符串，该字符串指定打印模板的URL。
		//V_VT(&cvarTemp) = VT_BSTR;
		//V_BSTR(&cvarTemp) = bstrURL;
		//_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_TemplateUrl")), cvarTemp, 0);
		//VariantClear(&cvarTemp);

		//__IE_uPrintFlags		
		//Retrieves a print flag value.
		//检索打印标志值。
		//---------------------------------
		V_VT(&cvarTemp) = VT_I4;
		V_I4(&cvarTemp) = 0;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_uPrintFlags")), cvarTemp, 0);
		VariantClear(&cvarTemp);

	//	//__IE_TemporaryFiles		
	//	//Retrieves a reference to a list of temporary file names saved from this document.
	//	//检索对从此文档保存的临时文件名列表的引用。
		TCHAR shortPathName[MAX_PATH];
		GetShortPathName(FileName, shortPathName, MAX_PATH);
		sfArray = SafeArrayCreateVector(VT_BSTR, 0, 1);
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = SysAllocString(shortPathName);
		LONG index = 0;
		SafeArrayPutElement(sfArray, &index, &cvarTemp);
		VariantClear(&cvarTemp);
		V_VT(&cvarTemp) = VT_ARRAY;
		V_ARRAY(&cvarTemp) = sfArray;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_TemporaryFiles")), cvarTemp, 0);
		VariantClear(&cvarTemp);
		SafeArrayDestroy(sfArray);

		//__IE_ParentHWND
		//---------------------------------
		V_VT(&cvarTemp) = VT_I4;
		V_I4(&cvarTemp) = 0;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_ParentHWND")), cvarTemp, 0);
		VariantClear(&cvarTemp);

		//__IE_DisplayUrl			
		//Retrieves a string that specifies theURLof the document.
		//检索一个字符串，该字符串指定文档的URL。
		//---------------------------------
		BSTR locationURL;
		iWebBrowser->get_LocationURL(&locationURL);
		V_VT(&cvarTemp) = VT_BSTR;
		V_BSTR(&cvarTemp) = locationURL;
		_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_DisplayUrl")), cvarTemp, 0);
		VariantClear(&cvarTemp);

		VARIANT  varArgs, varReturn;
		VariantInit(&varArgs);
		V_VT(&varArgs) = VT_UNKNOWN;
		V_UNKNOWN(&varArgs) = _pHtmlEvent2;

		//TCHAR    szTemp[MAX_PATH * 2];

		int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
		_stprintf_s(pOptions, MAX_PATH, _T("help:no;status:no;scroll:no;dialogLeft:0px;dialogTop:0px;dialogWidth:%dpx;dialogHeight:%dpx;"), cx - 15, cy);

	//	if FPrint then
	//		begin
	//		FWindowParams : = '';
	//FDialogFlags: = HTMLDLG_ALLOW_UNKNOWN_THREAD or HTMLDLG_NOUI or HTMLDLG_MODELESS or HTMLDLG_PRINT_TEMPLATE;
	//	end
	//	else
	//		begin
	//		FWindowParams : = 'resizable=yes;';
	//FDialogFlags: = HTMLDLG_ALLOW_UNKNOWN_THREAD or HTMLDLG_MODAL or HTMLDLG_MODELESS or HTMLDLG_PRINT_TEMPLATE;
	//	end;
		DWORD dialogFlags = HTMLDLG_ALLOW_UNKNOWN_THREAD | HTMLDLG_MODAL | HTMLDLG_MODELESS | HTMLDLG_PRINT_TEMPLATE;
		//::ShowHTMLDialogEx(0, pURLMoniker, dialogFlags, &varArgs, pOptions, &varReturn);

		HINSTANCE hinstMSHTML = LoadLibrary(TEXT("MSHTML.DLL"));
		if (hinstMSHTML != NULL)
		{
			SHOWHTMLDIALOGEXFN* pfnShowHTMLDialogEx;
			pfnShowHTMLDialogEx = (SHOWHTMLDIALOGEXFN*)GetProcAddress(hinstMSHTML, "ShowHTMLDialogEx");
			if (pfnShowHTMLDialogEx)
			{
				if (pURLMoniker)
				{
					DWORD dwFlags = HTMLDLG_MODELESS | HTMLDLG_VERIFY;
					(*pfnShowHTMLDialogEx)(NULL, pURLMoniker, dialogFlags, &varArgs, pOptions, &varReturn);
					pURLMoniker->Release();
				}
			}
		}
		FreeLibrary(hinstMSHTML);

		//TCHAR printerName[MAX_URL];
		//if (!GetDefaultPrinterInfo(printerName)) {
		//	MessageBox(NULL, _T("没有设置默认打印机"), _T("系统提示"), MB_OK);
		//}
		//else {
		//	TCHAR printerSettingA[MAX_URL];
		//	if (GetPrintSettingInfo(printerName, printerSettingA)) {
		//		MessageBox(NULL, printerSettingA, _T("系统提示"), MB_OK);
		//	}
		//	else {
		//		MessageBox(NULL, _T("获取打印机设置失败"), _T("系统提示"), MB_OK);
		//	}
		//	TCHAR printerSetting[MAX_URL];
		//	GetWindowText(hWndEditPrinterSetOld, printerSetting, MAX_URL);
		//	if (SetPrintSettingInfo(printerName, printerSetting)) {
		//		MessageBox(NULL, printerSetting, _T("系统提示"), MB_OK);
		//	}
		//	else {
		//		MessageBox(NULL, _T("设置打印机设置失败"), _T("系统提示"), MB_OK);
		//	}
		//	TCHAR printerSettingC[MAX_URL];
		//	if (GetPrintSettingInfo(printerName, printerSettingC)) {
		//		MessageBox(NULL, printerSettingC, _T("系统提示"), MB_OK);
		//	}
		//	else {
		//		MessageBox(NULL, _T("获取打印机设置失败"), _T("系统提示"), MB_OK);
		//	}
		//}

		if (hr != S_OK) {
			MessageBox(NULL, _T("QueryInterface IID_IHTMLEventObj2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
		}
		SysFreeString(bstrURL);
	}
	break;
	default:
		READYSTATE r;
		if (bolLoad && NULL != iWebBrowser) {
			hr = iWebBrowser->get_ReadyState(&r);
			switch (r)
			{
			case 0:
				printf("READYSTATE_UNINITIALIZED\n");
				break;
			case 1:
				printf("READYSTATE_LOADING\n");
				break;
			case 2:
				printf("READYSTATE_LOADED\n");
				break;
			case 3:
				printf("READYSTATE_INTERACTIVE\n");
				break;
			case 4:
				printf("READYSTATE_COMPLETE\n");
				bolLoad = FALSE;
				SendMessage(hWnd, IWB_DOCUMENTCOMPLETE, NULL, NULL);
				break;
			default:printf("default\n");
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
	static TCHAR szAppName[] = TEXT("WebBrowser");
	static TCHAR szClassName[] = TEXT("WebBrowser");
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Error!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hInstWindow = hInst;
	hWndWindow = CreateWindow(szClassName, szAppName, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_VISIBLE, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	ShowWindow(hWndWindow, SW_SHOW | nCmdShow);
	UpdateWindow(hWndWindow);

	//ShowWindow(hWndWindow, SW_HIDE);
	//UpdateWindow(hWndWindow);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}