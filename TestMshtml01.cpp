//#include <windows.h>
//#include <urlmon.h>
//#include <mshtmhst.h>
//#include <tchar.h>
//
//#pragma    comment(lib, "urlmon.lib")
//#define MAX_PRINTTEMPLATE_PARAMS					3000
//
//TCHAR pOptions[MAX_PATH];
//TCHAR pTemplateParams[MAX_PRINTTEMPLATE_PARAMS];
//
//int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
//{
//	HINSTANCE   hinstMSHTML = LoadLibrary(TEXT("MSHTML.DLL"));
//    if(hinstMSHTML)
//    {
//        SHOWHTMLDIALOGFN* pfnShowHTMLDialog;
//        pfnShowHTMLDialog = (SHOWHTMLDIALOGFN*)GetProcAddress(hinstMSHTML, "ShowHTMLDialog");
//        void* p = GetProcAddress(hinstMSHTML, "navigate");
//        if (pfnShowHTMLDialog)
//        {
//            TCHAR    szTemp[MAX_PATH * 2];
//            IMoniker* pmk = NULL;
//            LPCTSTR   str = _T("res://ietest.exe/PT.HTM");
//            if (SUCCEEDED(::CreateURLMoniker(NULL, str, &pmk)))
//            {
//                HRESULT  hr;
//                VARIANT  varArgs, varReturn;
//                VariantInit(&varReturn);
//                //_stprintf_s(pTemplateParams, MAX_PRINTTEMPLATE_PARAMS, _T("printType:Preview;contentDocumentUrl:%S;"), enUrl.c_str());
//                varArgs.vt = VT_BSTR;
//                varArgs.bstrVal = SysAllocString(_T("printType:Preview;contentDocumentUrl:%S;"));
//
//                int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
//				int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
//                _stprintf_s(pOptions, MAX_PATH, _T("help:no;status:no;scroll:no;dialogLeft:0px;dialogTop:0px;dialogWidth:%dpx;dialogHeight:%dpx;"), cx - 15, cy);
//                hr = (*pfnShowHTMLDialog)(NULL, pmk, &varArgs, pOptions, &varReturn);
//                VariantClear(&varArgs);
//                pmk->Release();
//
//                if (SUCCEEDED(hr))
//                {
//                    switch (varReturn.vt)
//                    {
//                    case VT_BSTR:
//                    {
//                        TCHAR szData[MAX_PATH];
//                        wsprintf(szTemp, TEXT("The selected item was \"%s\"."), szData);
//                        VariantClear(&varReturn);
//                        break;
//                    }
//                    default:
//                        lstrcpy(szTemp, TEXT("Cancel was selected."));
//                        break;
//                    }
//                    MessageBox(NULL, szTemp, TEXT("HTML Dialog Sample"), MB_OK | MB_ICONINFORMATION);
//                }
//                else
//                    MessageBox(NULL, TEXT("ShowHTMLDialog Failed."), TEXT("HTML Dialog Sample"), MB_OK | MB_ICONERROR);
//
//            }
//        }
//        FreeLibrary(hinstMSHTML);
//    }
//    return 0;
//}