//#include <atlbase.h>
//#include <atlcom.h>
//#include <string>
//CComModule _Module;
//#include <atlwin.h>
//#include <atlstr.h>
//
//
//#pragma comment(lib,"atls")
//#pragma comment(lib,"User32.lib")
//
//#define MAX_LOADSTRING			100
//#define MAX_URL					1000
//
//#define IDB_GOURL				3301
//#define IDB_PRINT				3302
//#define IDB_PRINTPREVIEW		3303
//
//VOID GotoUrl();
//#pragma    comment(lib, "urlmon.lib")
//
//// ȫ�ֱ���:
//HINSTANCE hInst;								// ��ǰʵ��
//TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
//TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
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
//	CComQIPtr<IServiceProvider> isp;
//	//iWebBrowser->get_Document((IDispatch**)&isp);
//	//CComQIPtr<IServiceProvider> isp = (IHTMLDocument2*)m_IE.GetDocument();
//	//CComQIPtr<IWebBrowser2> pWB2;
//	//isp->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&pWB2);
//
//	iWebBrowser->Release();
//}
//
///*  ����win32 ��שӦ�ó���ģ��*/
////��һ�»ص�����
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
//		case IDB_GOURL:
//			TCHAR buf[MAX_URL];
//			GetWindowText(hWndEditUrl, buf, MAX_URL);
//			GotoUrl(buf);
//
//			IOleCommandTarget* pCmdTarg;
//			hr = iWebBrowser->QueryInterface(IID_IOleCommandTarget, (void**)&pCmdTarg);
//
//			VARIANT vTemplate;
//			V_VT(&vTemplate) = VT_BSTR;
//			V_BSTR(&vTemplate) = SysAllocString(_T("res://ietest.exe/PT.HTM"));
//
//			pCmdTarg->Exec(&CGID_MSHTML, 27, OLECMDEXECOPT_PROMPTUSER, &vTemplate, NULL);
//			//CComPtr<IHTMLEventObj> pEventObj;
//			//hr = pWB2->QueryInterface(IID_IHTMLEventObj, (LPVOID*)&pEventObj);
//
//			//CComPtr<IHTMLEventObj2> pEvObj2 = NULL;
//			//hr = pEventObj->QueryInterface(IID_IHTMLEventObj2, (void**)&pEvObj2);
//			//VARIANT cvarTemp;
//			//				// Set the template URL
//			//			//---------------------------------
//			//			V_VT(&cvarTemp) = VT_BSTR;
//			//			V_BSTR(&cvarTemp) = SysAllocString(_T("http://www.baidu.com"));
//			//			pEvObj2->setAttribute(SysAllocString(_T("__IE_ContentDocumentUrl")), cvarTemp, 0);
//			//			VariantClear(&cvarTemp);
//
//			//			// set the Header string
//			//			//---------------------------------
//			//			V_VT(&cvarTemp) = VT_BSTR;
//			//			V_BSTR(&cvarTemp) = SysAllocString(_T("Header"));;
//			//			pEvObj2->setAttribute(SysAllocString(_T("__IE_HeaderString")), cvarTemp, 0);
//			//			VariantClear(&cvarTemp);
//
//			//			V_VT(&cvarTemp) = VT_BSTR;
//			//			V_BSTR(&cvarTemp) = SysAllocString(_T("Preview"));
//			//			pEvObj2->setAttribute(SysAllocString(_T("__IE_PrintType")), cvarTemp, 0);
//			//			VariantClear(&cvarTemp);
//
//			//IHostDialogHelper* pHDH;
//			//IMoniker* pUrlMoniker;
//			//BSTR bstrOptions = SysAllocString(_T("dialogHeight:30;dialogWidth:40"));
//			//BSTR bstrPath = SysAllocString(_T("res://ietest.exe/PT.HTM"));
//			//TCHAR pOptions[MAX_PATH];
//			//int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
//			//int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
//			//_stprintf_s(pOptions, MAX_PATH, _T("help:no;status:no;scroll:no;dialogLeft:0px;dialogTop:0px;dialogWidth:%dpx;dialogHeight:%dpx;"), cx - 15, cy);
//			//CreateURLMoniker(NULL, bstrPath, &pUrlMoniker);
//			//CoCreateInstance(CLSID_HostDialogHelper, NULL, CLSCTX_INPROC, IID_IHostDialogHelper, (void**)&pHDH);
//			//pHDH->ShowHTMLDialog(NULL, pUrlMoniker, NULL, bstrOptions, NULL, NULL);
//
//			//SysFreeString(bstrPath);
//			//SysFreeString(bstrOptions);
//			//pUrlMoniker->Release();
//			//pHDH->Release();
//
//			//ShellExecute(NULL, "open", pNMHDR->sURLLink, NULL, NULL, SW_SHOWNORMAL);	//��ԭIE����������д�
//			//ShellExecute(hWndWindow, _T("open"),  _T("IEXPLORE"), _T("file:///F:/WorkDocument/hntp/���/topprint/topprintn.html"),  NULL,   SW_SHOWNORMAL);	//���µ�����������д�
//			//ShellExecute(hWndWindow, _T("print"), _T("res://ietest.exe/PT.HTM"), NULL, NULL, SW_SHOWNORMAL);
//			break;
//		case IDB_PRINT:
//			GetWindowText(hWndEditUrl, buf, MAX_URL);
//			ret = GetSystemDirectory(szSystemPath, MAX_PATH);
//			if (!ret) {
//				printf("GetSystemDirectory fail (%ld)\n", GetLastError());
//				break;
//			}
//			len = (sizeof(szSystemPath) + sizeof(_T("\\mshtml.dll,PrintHTML \"")) + sizeof(buf) + sizeof(_T("\"")))/ sizeof(TCHAR);
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
//			//HINSTANCE hinstMSHTML = LoadLibrary(TEXT("MSHTML.DLL"));
//			//if (hinstMSHTML != NULL)
//			//{
//			//	SHOWHTMLDIALOGFN* pfnShowHTMLDialog;
//			//	pfnShowHTMLDialog = (SHOWHTMLDIALOGFN*)GetProcAddress(hinstMSHTML, "ShowHTMLDialog");
//			//	if (pfnShowHTMLDialog)
//			//	{
//			//		IMoniker* pURLMoniker;
//			//		BSTR bstrURL = SysAllocString(L"res://ietest.exe/HTML/PT.HTM");
//			//		CreateURLMoniker(NULL, bstrURL, &pURLMoniker);
//			//		if (pURLMoniker)
//			//		{
//			//			(*pfnShowHTMLDialog)(NULL, pURLMoniker, NULL, NULL, NULL);
//			//			pURLMoniker->Release();
//			//		}
//
//			//		SysFreeString(bstrURL);
//			//	}
//			//	FreeLibrary(hinstMSHTML);
//			//}
//		}
//			break;
//		default:
//			break;
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		GetClientRect(hWnd, &rc);
//		//��һ�������Ի���ʱ�Ĵ�С�仯��������
//		if (0 == rc.left && 0 == rc.right&& 0 == rc.top && 0 == rc.bottom)
//		{
//		}
//		else
//		{
//			//����ǰ�������С�����򴥷���������ʱ������Ի�������
//			if (0 != rc.right && 0 != rc.bottom)
//			{
//				//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С
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
//		WinContainer.QueryControl(__uuidof(IWebBrowser2), (void**)& iWebBrowser);
//
//		//GotoUrl(_T("file:///F:/WorkDocument/hntp/���/topprint/topprint.html"));
//		hWndEditUrl = CreateWindow(_T("edit"), _T("file:///F:/WorkDocument/hntp/���/topprint/topprintn.html"), WS_CHILD | WS_VISIBLE | WS_BORDER, 10, rc.bottom - 50, 800, 40, hWnd, NULL, NULL, NULL);
//		hWndBtnGo = CreateWindow(_T("Button"), _T("����"), WS_VISIBLE | WS_CHILD, 820, rc.bottom - 50, 120, 40, hWnd, (HMENU)IDB_GOURL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
//		hWndBtnPrint = CreateWindow(_T("Button"), _T("��ӡ"), WS_VISIBLE | WS_CHILD, 950, rc.bottom-50, 120, 40, hWnd, (HMENU)IDB_PRINT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
//		hWndBtnPrintPreview = CreateWindow(_T("Button"), _T("��ӡԤ��"), WS_VISIBLE | WS_CHILD, 1080, rc.bottom - 50, 120, 40, hWnd, (HMENU)IDB_PRINTPREVIEW, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//
//}
//
//////int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
////int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
////{
////	static TCHAR szAppName[] = TEXT("WebBrowser");
////	static TCHAR szClassName[] = TEXT("WebBrowser");
////	MSG msg;
////	WNDCLASS wndclass;
////	wndclass.style = CS_HREDRAW | CS_VREDRAW;
////	wndclass.lpfnWndProc = WndProc;
////	wndclass.cbClsExtra = 0;
////	wndclass.cbWndExtra = 0;
////	wndclass.hInstance = hInst;
////	wndclass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
////	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
////	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
////	wndclass.lpszMenuName = NULL;
////	wndclass.lpszClassName = szClassName;
////
////	if (!RegisterClass(&wndclass))
////	{
////		MessageBox(NULL, TEXT("Error!"), szAppName, MB_ICONERROR);
////		return 0;
////	}
////	/*
////	API ����ԭ�ͣ�
////HWND WINAPI CreateWindow(
////  _In_opt_  LPCTSTR lpClassName,      // ����������
////  _In_opt_  LPCTSTR lpWindowName,  // ���ڱ���
////  _In_      DWORD dwStyle,                // ���ڷ�񣬻�ƴ��ڸ�ʽ
////  _In_      int x,                                 // ��ʼ x ����
////  _In_      int y,                                 // ��ʼ y ����
////  _In_      int nWidth,                        // ��ʼ x ����ߴ�
////  _In_      int nHeight,                       // ��ʼ y ����ߴ�
////  _In_opt_  HWND hWndParent,        // �����ھ��
////  _In_opt_  HMENU hMenu,               // ���ڲ˵����
////  _In_opt_  HINSTANCE hInstance,    // ����ʵ�����
////  _In_opt_  LPVOID lpParam              // ��������
////);
////	*/
////	hInstWindow = hInst;
////	//WS_OVERLAPPEDWINDOW������һ������WS_OVERLAPPED��WS_CAPTION��WS_SYSMENU��WS_THICKFRAME
////	//WS_MINIMIZEBOX��WS_MAXMIZEBOX���Ĳ�����ڡ���WS_OVERLAPPEDWINDOW�����ͬ��
////	//WS_VISIBLE������һ����ʼ״̬Ϊ�ɼ��Ĵ��ڡ�
////	hWndWindow = CreateWindow(szClassName, szAppName, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_VISIBLE, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
////	ShowWindow(hWndWindow, SW_SHOW|nCmdShow);
////	UpdateWindow(hWndWindow);
////
////	//ShowWindow(hWndWindow, SW_HIDE);
////	//UpdateWindow(hWndWindow);
////	while (GetMessage(&msg, NULL, 0, 0))
////	{
////		TranslateMessage(&msg);
////		DispatchMessage(&msg);
////	}
////	return msg.wParam;
////}