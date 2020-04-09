//#include <atlbase.h>
//#include <atlcom.h>
//#include <string>
//CComModule _Module;
//#include <atlwin.h>
//#include <atlstr.h>
//
//#pragma comment(lib,"atls")
//#pragma comment(lib,"User32.lib")
//#pragma comment(lib,"urlmon.lib")
////#import "C:\\windows\\system32\\mshtml.tlb" // location of mshtml.tlb
//
//#define MAX_LOADSTRING			100
//#define MAX_URL					1000
//
//#define IDB_GOURL				3301
//#define IDB_PRINT				3302
//#define IDB_PRINTPREVIEW		3303
//
//
//#define MAX_PRINTTEMPLATE_PARAMS					3000
//
//TCHAR pOptions[MAX_PATH];
//TCHAR pTemplateParams[MAX_PRINTTEMPLATE_PARAMS];
//
//
//// 全局变量:
//HINSTANCE hInst;								// 当前实例
//TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
//TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
//TCHAR szSystemPath[MAX_PATH];
//TCHAR buf[MAX_URL];
//
//DWORD ret;
//HINSTANCE hInstWindow;
//HWND hWndWindow, hWndWebBrower, hWndEditUrl, hWndBtnGo, hWndBtnPrint, hWndBtnPrintPreview;
//TCHAR* n_str;
//size_t len;
//
//IWebBrowser2* iWebBrowser;
//VARIANT varMyURL;
//CString m_TemplateURL;
//
//HRESULT  hr;
//
//BEGIN_OBJECT_MAP(ObjectMap)
//
//END_OBJECT_MAP()
//
//VOID GotoUrl(CString url) {
//	VariantInit(&varMyURL);
//	varMyURL.vt = VT_BSTR;
//	varMyURL.bstrVal = SysAllocString(url);
//	iWebBrowser->Navigate2(&varMyURL, 0, 0, 0, 0);
//	VariantClear(&varMyURL);
//	iWebBrowser->Release();
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//	//*********************
//	RECT rc;
//	static CAxWindow WinContainer;
//
//	CString strFunction = _T("shell.Explorer.2");
//	BSTR b = strFunction.AllocSysString();
//	LPOLESTR pszName = b;
//
//	GetClientRect(hWnd, &rc);
//
//	GUID guid;
//	//**********************
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		switch (LOWORD(wParam))
//		{
//		case IDB_GOURL: {
//			TCHAR buf[MAX_URL];
//			GetWindowText(hWndEditUrl, buf, MAX_URL);
//			GotoUrl(buf);
//			IDispatch* pDp = NULL;
//
//			hr = iWebBrowser->get_Document(&pDp);
//			if (hr != S_OK) {
//				MessageBox(NULL, _T("DWebBrowser2::get_Document 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			}
//			IHTMLDocument4* _pHtmlDoc4;
//			hr = pDp->QueryInterface(IID_IHTMLDocument4, (void**)&_pHtmlDoc4);
//			if (hr != S_OK) {
//				MessageBox(NULL, _T("QueryInterface IID_IHTMLDocument2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			}
//			//IHTMLWindow2* _pHtmlWnd2;
//			//hr = _pHtmlDoc2->get_parentWindow(&_pHtmlWnd2);
//			//if (hr != S_OK) {
//			//	MessageBox(NULL, _T("IHTMLDocument2::get_parentWindow 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			//}
//
//			IHTMLEventObj* _pHtmlEvent;
//			VARIANT vName, vObj;
//			hr = _pHtmlDoc4->createEventObject(&vName, &_pHtmlEvent);
//			//hr = _pHtmlWnd2->get_event(&_pHtmlEvent);
//			if (hr != S_OK) {
//				MessageBox(NULL, _T("IHTMLWindow2::get_event 错误"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			}
//			IHTMLEventObj2* _pHtmlEvent2;
//			hr = _pHtmlEvent->QueryInterface(IID_IHTMLEventObj2, (void**)&_pHtmlEvent2);
//			if (hr != S_OK) {
//				MessageBox(NULL, _T("QueryInterface IID_IHTMLEventObj2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			}
//			VARIANT cvarTemp;
//			SAFEARRAY* sfArray;
//			// Creates a new one-dimensional array
//			sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
//
//			// Set the template URL
//			//---------------------------------
//			V_VT(&cvarTemp) = VT_BSTR;
//			V_BSTR(&cvarTemp) = SysAllocString(_T("http://www.baidu.com"));
//			_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_ContentDocumentUrl")), cvarTemp, 0);
//			VariantClear(&cvarTemp);
//
//			// set the Header string
//			//---------------------------------
//			V_VT(&cvarTemp) = VT_BSTR;
//			V_BSTR(&cvarTemp) = SysAllocString(_T("Header"));;
//			_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_HeaderString")), cvarTemp, 0);
//			VariantClear(&cvarTemp);
//
//			// set the Footer string
//			//---------------------------------
//			V_VT(&cvarTemp) = VT_BSTR;
//			V_BSTR(&cvarTemp) = SysAllocString(_T("Footer"));;
//			_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_FooterString")), cvarTemp, 0);
//			VariantClear(&cvarTemp);
//
//			V_VT(&cvarTemp) = VT_BSTR;
//			V_BSTR(&cvarTemp) = SysAllocString(_T("Preview"));
//			_pHtmlEvent2->setAttribute(SysAllocString(_T("__IE_PrintType")), cvarTemp, 0);
//			VariantClear(&cvarTemp);
//
//			VARIANT  varArgs, varReturn;
//			VariantInit(&varArgs);
//			V_VT(&varArgs) = VT_UNKNOWN;
//			V_UNKNOWN(&varArgs) = _pHtmlEvent2;
//
//			IMoniker* pURLMoniker;
//			BSTR bstrURL = SysAllocString(_T("res://ietest.exe/PT.HTM"));
//			if (!SUCCEEDED(::CreateURLMoniker(NULL, bstrURL, &pURLMoniker))) {
//				MessageBox(NULL, TEXT("CreateURLMoniker Failed."), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONERROR);
//				break;
//			}
//
//			TCHAR    szTemp[MAX_PATH * 2];
//
//			int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
//			int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
//			_stprintf_s(pOptions, MAX_PATH, _T("help:no;status:no;scroll:no;dialogLeft:0px;dialogTop:0px;dialogWidth:%dpx;dialogHeight:%dpx;"), cx - 15, cy);
//
//			hr = ::ShowHTMLDialog(NULL, pURLMoniker, &varArgs, pOptions, &varReturn);
//			VariantClear(&varArgs);
//			pURLMoniker->Release();
//
//			if (SUCCEEDED(hr))
//			{
//				switch (varReturn.vt)
//				{
//				case VT_BSTR:
//				{
//					TCHAR szData[MAX_PATH];
//					wsprintf(szTemp, TEXT("The selected item was \"%s\"."), szData);
//					VariantClear(&varReturn);
//					break;
//				}
//				default:
//					lstrcpy(szTemp, TEXT("Cancel was selected."));
//					break;
//				}
//				MessageBox(NULL, szTemp, TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			}
//			else
//			{
//				MessageBox(NULL, TEXT("ShowHTMLDialog Failed."), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONERROR);
//
//			}
//			
//
//			//hr = ::ShowHTMLDialog(NULL, pURLMoniker, &varArgs, pOptions, &varReturn);
//			//hr = ShowHTMLDialog(NULL, pURLMoniker, NULL, pOptions, NULL);
//			if (hr != S_OK) {
//				MessageBox(NULL, _T("QueryInterface IID_IHTMLEventObj2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//			}
//			SysFreeString(bstrURL);
//			break;
//		}
//		case IDB_PRINT:
//			GetWindowText(hWndEditUrl, buf, MAX_URL);
//			ret = GetSystemDirectory(szSystemPath, MAX_PATH);
//			if (!ret) {
//				printf("GetSystemDirectory fail (%ld)\n", GetLastError());
//				break;
//			}
//			len = (sizeof(szSystemPath) + sizeof(_T("\\mshtml.dll,PrintHTML \"")) + sizeof(buf) + sizeof(_T("\""))) / sizeof(TCHAR);
//			n_str = new TCHAR[len + 1];
//			_tcscpy_s(n_str, len, szSystemPath);
//			_tcscat_s(n_str, len, _T("\\mshtml.dll,PrintHTML \""));
//			_tcscat_s(n_str, len, buf);
//			_tcscat_s(n_str, len, _T("\""));
//			ShellExecute(0, NULL, _T("rundll32.exe"), n_str, NULL, SW_NORMAL);
//			break;
//		case IDB_PRINTPREVIEW:
//		{
//			GetWindowText(hWndEditUrl, buf, MAX_URL);
//			ret = GetSystemDirectory(szSystemPath, MAX_PATH);
//			if (!ret) {
//				printf("GetSystemDirectory fail (%ld)\n", GetLastError());
//				break;
//			}
//			len = (sizeof(szSystemPath) + sizeof(_T("\\mshtml.dll,ShowHTMLDialog \"res://ietest.exe/PT.HTM\""))) / sizeof(TCHAR);
//			n_str = new TCHAR[len + 1];
//			_tcscpy_s(n_str, len, szSystemPath);
//			_tcscat_s(n_str, len, _T("\\mshtml.dll,ShowHTMLDialog \"res://ietest.exe/PT.HTM\""));
//			ShellExecute(0, NULL, _T("rundll32.exe"), n_str, NULL, SW_NORMAL);
//		}
//		break;
//		default:
//			break;
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		GetClientRect(hWnd, &rc);
//		//第一次启动对话框时的大小变化不做处理
//		if (0 == rc.left && 0 == rc.right && 0 == rc.top && 0 == rc.bottom)
//		{
//		}
//		else
//		{
//			//如果是按下了最小化，则触发条件，这时不保存对话框数据
//			if (0 != rc.right && 0 != rc.bottom)
//			{
//				//重绘函数，用以更新对话框上控件的位置和大小
//				MoveWindow(hWndWebBrower, rc.left, rc.top, rc.right, rc.bottom - 60, true);
//				MoveWindow(hWndEditUrl, 10, rc.bottom - 50, 800, 40, true);
//				MoveWindow(hWndBtnGo, 820, rc.bottom - 50, 120, 40, true);
//				MoveWindow(hWndBtnPrint, 950, rc.bottom - 50, 120, 40, true);
//				MoveWindow(hWndBtnPrintPreview, 1080, rc.bottom - 50, 120, 40, true);
//			}
//		}
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_CREATE:
//		_Module.Init(ObjectMap, hInst, &guid);
//		hWndWebBrower = WinContainer.Create(hWnd, rc, 0, WS_CHILD | WS_VISIBLE);
//		WinContainer.CreateControl(pszName);
//		WinContainer.QueryControl(__uuidof(IWebBrowser2), (void**)&iWebBrowser);
//		hWndEditUrl = CreateWindow(_T("edit"), _T("file:///F:/WorkDocument/hntp/插件/topprint/topprintn.html"), WS_CHILD | WS_VISIBLE | WS_BORDER, 10, rc.bottom - 50, 800, 40, hWnd, NULL, NULL, NULL);
//		hWndBtnGo = CreateWindow(_T("Button"), _T("访问"), WS_VISIBLE | WS_CHILD, 820, rc.bottom - 50, 120, 40, hWnd, (HMENU)IDB_GOURL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
//		hWndBtnPrint = CreateWindow(_T("Button"), _T("打印"), WS_VISIBLE | WS_CHILD, 950, rc.bottom - 50, 120, 40, hWnd, (HMENU)IDB_PRINT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
//		hWndBtnPrintPreview = CreateWindow(_T("Button"), _T("打印预览"), WS_VISIBLE | WS_CHILD, 1080, rc.bottom - 50, 120, 40, hWnd, (HMENU)IDB_PRINTPREVIEW, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
//{
//
//
//	//IMoniker* pURLMoniker;
//	//BSTR bstrURL = SysAllocString(_T("res://ietest.exe/PT.HTM"));
//	//::CreateURLMonikerEx(NULL, bstrURL, &pURLMoniker, URL_MK_UNIFORM);
//
//	//int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
//	//int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
//	//_stprintf_s(pOptions, MAX_PATH, _T("help:no;status:no;scroll:no;dialogLeft:0px;dialogTop:0px;dialogWidth:%dpx;dialogHeight:%dpx;"), cx - 15, cy);
//	////hr = ShowHTMLDialog(NULL, pURLMoniker, &varArgs, pOptions, &varReturn);
//	//hr = ::ShowHTMLDialog(NULL, pURLMoniker, NULL, pOptions, NULL);
//	//if (hr != S_OK) {
//	//	MessageBox(NULL, _T("QueryInterface IID_IHTMLEventObj2 失败"), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//	//}
//	//SysFreeString(bstrURL);
//
//
//	static TCHAR szAppName[] = TEXT("WebBrowser");
//	static TCHAR szClassName[] = TEXT("WebBrowser");
//	MSG msg;
//	WNDCLASS wndclass;
//	wndclass.style = CS_HREDRAW | CS_VREDRAW;
//	wndclass.lpfnWndProc = WndProc;
//	wndclass.cbClsExtra = 0;
//	wndclass.cbWndExtra = 0;
//	wndclass.hInstance = hInst;
//	wndclass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
//	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wndclass.lpszMenuName = NULL;
//	wndclass.lpszClassName = szClassName;
//
//	if (!RegisterClass(&wndclass))
//	{
//		MessageBox(NULL, TEXT("Error!"), szAppName, MB_ICONERROR);
//		return 0;
//	}
//	hInstWindow = hInst;
//	hWndWindow = CreateWindow(szClassName, szAppName, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_VISIBLE, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
//	ShowWindow(hWndWindow, SW_SHOW | nCmdShow);
//	UpdateWindow(hWndWindow);
//
//	//ShowWindow(hWndWindow, SW_HIDE);
//	//UpdateWindow(hWndWindow);
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//	return msg.wParam;
//}