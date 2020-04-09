//#include <atlbase.h>
//#include <atlwin.h>
//#include <atlhost.h>
//#include <windows.h>
//#include <stdio.h>
//#include <tchar.h>
//#include <urlmon.h>
//#include <MSHTML.h>
//#include <string>
//
////dumpbin (这个是visual studio 提供的工具或者可以安装 Dependency Walker)来查看 DLL 依赖关系
////比webprint
////-少
////WINSPOOL.DRV
////SHLWAPI.dll
////+多
////SHELL32.DLL
////urlmon.dll
////VCRUNTIME140D.dll
////ucrtbased.dll
//
//
//#pragma    comment(lib, "urlmon.lib")
//typedef   HRESULT   STDAPICALLTYPE   MYSHOWHTMLDIALOGFN
//(HWND   hwndParent, IMoniker
//	* pmk, VARIANT* pvarArgIn, TCHAR* pchOptions,
//	VARIANT* pvArgOut);
//#define MAX_PRINTTEMPLATE_PARAMS					3000
//
//
//TCHAR pOptions[MAX_PATH];
//TCHAR pTemplateParams[MAX_PRINTTEMPLATE_PARAMS];
//
//int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
//{
//	MYSHOWHTMLDIALOGFN* pFunc;
//	HINSTANCE hLib = ::LoadLibrary(_T("mshtml.dll"));
//	if (hLib != INVALID_HANDLE_VALUE)
//	{
//		pFunc = (MYSHOWHTMLDIALOGFN*)::GetProcAddress(hLib, "ShowHTMLDialog");
//		if (pFunc)
//		{
//			LPCWSTR   str = _T("res://ietest.exe/PT.HTM");
//			IMoniker* pmoniker = NULL;
//			if (SUCCEEDED(::CreateURLMoniker(NULL, str, &pmoniker)))
//			{
//				//要取得屏幕大小，可以用下面几个函数：
//				//# include <windows.h>
//				int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
//				int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
//				//通过上边两个函数获取的是 显示屏幕的大小，但不包括任务栏等区域。
//				//int  cx = GetSystemMetrics(SM_CXSCREEN);
//				//int  cy = GetSystemMetrics(SM_CYSCREEN);
//				//_tprintf() 输出不了汉字
//				//setlocale(LC_ALL, ""); //必须得有这行 否则不能输出中文 注意得加locale头文件
//				
//				//std::string enUrl = UrlEncode(std::string("http://www.baidu.com"));
//				//_stprintf_s(pTemplateParams, MAX_PRINTTEMPLATE_PARAMS, _T("printType:Preview;contentDocumentUrl:%S;"), enUrl.c_str());
//
//				_stprintf_s(pOptions, MAX_PATH, _T("help:no;status:no;scroll:no;dialogLeft:0px;dialogTop:0px;dialogWidth:%dpx;dialogHeight:%dpx;"), cx - 15, cy);
//				//size_t len = (sizeof(_T("help:no;status:no;scroll:no;dialogHeight:")) + sizeof(itow(cx)) + sizeof(_T("px;dialogWidth:")) + sizeof(buf) + sizeof(_T("px;"))) / sizeof(TCHAR);
//				//TCHAR* pOptions = new TCHAR[len + 1];
//				//_tcscpy_s(pOptions, len, _T("help:no;status:no;scroll:no;dialogHeight:"));
//				//_tcscat_s(pOptions, len, buf);
//				//_tcscat_s(pOptions, len, _T("px;dialogWidth:"));
//				//_tcscat_s(pOptions, len, buf);
//				//_tcscat_s(pOptions, len, _T("px;"));
//				//TCHAR* pOptions = (TCHAR*)L"__IE_PrintType:Preview;__IE_ContentDocumentUrl:http://www.baidu.com";
//				//这里必须是UNICODE型的字符串.否则更改不了窗口的大小
//
//				HRESULT  hr;
//				VARIANT  varArgs, varReturn;
//
//	//			//TemplateParams* templateParams = new TemplateParams;
//	//			//size_t len = sizeof(_T("Preview")) / sizeof(TCHAR);
//	//			//TCHAR* printType = new TCHAR[len + 1];
//	//			//templateParams->__IE_PrintType = printType;
//	//			//len = sizeof(_T("http://www.baidu.com")) / sizeof(TCHAR);
//	//			//TCHAR* contentDocumentUrl = new TCHAR[len + 1];
//	//			//templateParams->__IE_ContentDocumentUrl = contentDocumentUrl;
//
//	//			// ... code to create the IHTMLEventObj2 object, pEvObj2...
//	//			IHTMLEventObj* pEventObj = NULL;
//	//			IHTMLEventObj2* pEvObj2 = NULL;
//
//	//			// Create the event object as a holder for the parameters to pass
//	//			//---------------------------------
//	//			//IID_IHTMLEventObj
//
//	//			//CComPtr<IDispatch> spEventObj;
//	//			//hr = spEventObj->QueryInterface(IID_IHTMLEventObj, (LPVOID*)&pEventObj);
//
//	//			hr = ::LoadRegTypeLib(LIBID_ServerLib, 1, 0, 0x00, pEventObj);
//
//
//	//			//hr = CEventObj::Create(&pEventObj, NULL, NULL, NULL, FALSE, NULL, NULL, TRUE);
//	//			hr = pEventObj->QueryInterface(IID_IHTMLEventObj2, (void**)&pEvObj2);
//
//	//			VARIANT cvarTemp;
//
//	//			// Set the template URL
//	//			//---------------------------------
//	//			V_VT(&cvarTemp) = VT_BSTR;
//	//			V_BSTR(&cvarTemp) = SysAllocString(_T("http://www.baidu.com"));
//	//			pEvObj2->setAttribute(SysAllocString(_T("__IE_ContentDocumentUrl")), cvarTemp, 0);
//	//			VariantClear(&cvarTemp);
//
//	//			// set the Header string
//	//			//---------------------------------
//	//			V_VT(&cvarTemp) = VT_BSTR;
//	//			V_BSTR(&cvarTemp) = SysAllocString(_T("Header"));;
//	//			pEvObj2->setAttribute(SysAllocString(_T("__IE_HeaderString")), cvarTemp, 0);
//	//			VariantClear(&cvarTemp);
//
//	//			V_VT(&cvarTemp) = VT_BSTR;
//	//			V_BSTR(&cvarTemp) = SysAllocString(_T("Preview"));
//	//			pEvObj2->setAttribute(SysAllocString(_T("__IE_PrintType")), cvarTemp, 0);
//	//			VariantClear(&cvarTemp);
//
//	///*			varArgs.vt = VT_BYREF;
//	//			varArgs.byref = pEvObj2;*/
//	//			
//	//			V_UNKNOWN(&varArgs) = pEvObj2;
//
//				VariantInit(&varReturn);
//				//hr = (*pfnShowHTMLDialog)(NULL, pmk, &varArgs, L"resizable:1", &varReturn);
//				//hr = pFunc(NULL, pmoniker, NULL, pOptions, NULL);
//				V_VT(&varArgs) = VT_BSTR;
//				V_BSTR(&varArgs) = SysAllocString(pTemplateParams);;
//				hr = pFunc(NULL, pmoniker, &varArgs, pOptions, &varReturn);
//				VariantClear(&varArgs);
//				pmoniker->Release();
//
//				TCHAR    szTemp[MAX_PATH * 2];
//
//				if (SUCCEEDED(hr))
//				{
//					switch (varReturn.vt)
//					{
//					case VT_BSTR:
//					{
//						TCHAR szData[MAX_PATH];
//						wsprintf(szTemp, TEXT("The selected item was \"%s\"."), szData);
//						VariantClear(&varReturn);
//						break;
//					}
//					default:
//						break;
//					}
//					MessageBox(NULL, szTemp, TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//				}
//				else {
//					MessageBox(NULL, TEXT("ShowHTMLDialog Failed."), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONERROR);
//				}
//			}
//			else
//				MessageBox(NULL, _T("CreateURLMoniker fails"), _T("Error.."), MB_OK);
//		}
//		else
//			MessageBox(NULL, _T("GetProcAddress fails"), _T("Error.."), MB_OK);
//		::FreeLibrary(hLib);
//	}
//	else {
//		MessageBox(NULL, _T("LoadLibrary fails"), _T("Error.."), MB_OK);
//	}
//	return 0;
//
//	//IHostDialogHelper* pHDH;
//	//IMoniker* pUrlMoniker;
//	//BSTR bstrOptions = SysAllocString(L"dialogHeight:30;dialogWidth:40");
//	//BSTR bstrPath = SysAllocString(L"c:\\dummy.htm");
//
//	//CreateURLMoniker(NULL, bstrPath, &pUrlMoniker);
//	//CoCreateInstance(CLSID_HostDialogHelper, NULL, CLSCTX_INPROC, IID_IHostDialogHelper, (void**)&pHDH);
//	//pHDH->ShowHTMLDialog(NULL, pUrlMoniker, NULL, bstrOptions, NULL, NULL);
//
//	//SysFreeString(bstrPath);
//	//SysFreeString(bstrOptions);
//	//pUrlMoniker->Release();
//	//pHDH->Release();
//}