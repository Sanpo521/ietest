//DoIEControl.cpp

#include "stdafx.h"

static HHOOK _hMouseHook;
static HHOOK _hKeyBoardHook;
static HWND _hWndMain;

static LRESULT CALLBACK MouseProc(
    int nCode,      // hook code
    WPARAM wParam,  // message identifier
    LPARAM lParam   // mouse coordinates
    );

static LRESULT CALLBACK KeyBoardProc(
    int nCode,      // hook code
    WPARAM wParam,  // message identifier
    LPARAM lParam   // mouse coordinates
    );


bool Init_DisableIEContextMenu(HWND hWndMain)
{
    HHOOK hMouseHook;
    HHOOK hKeyBoardHook;
    hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
    hKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, NULL, GetCurrentThreadId());
    if (NULL != hMouseHook)
    {
        _hMouseHook = hMouseHook;
        _hKeyBoardHook = hKeyBoardHook;
        _hWndMain = hWndMain;
        return true;
    }
    return false;
}

void Free_DisableIEContextMenu(void)
{
    if (NULL != _hMouseHook)
    {
        UnhookWindowsHookEx(_hMouseHook);
        _hMouseHook = NULL;
    }
    if (NULL != _hKeyBoardHook)
    {
        UnhookWindowsHookEx(_hKeyBoardHook);
        _hKeyBoardHook = NULL;
    }
}

// 取得顶级窗口
static HWND GetTopLevelHWND(HWND hWnd)
{
    HWND hWndParent = GetParent(hWnd);

    while (NULL != hWndParent)
    {
        hWnd = hWndParent;
        hWndParent = GetParent(hWnd);
    }

    return hWnd;
}

static LRESULT CALLBACK MouseProc(
    int nCode,      // hook code
    WPARAM wParam,  // message identifier
    LPARAM lParam   // mouse coordinates
    )
{
    char lpszClassName[1024];
    //HWND hwndFocus=GetFocus();
    POINT pointMouse;
    GetCursorPos(&pointMouse);
    HWND hWnd = ::WindowFromPoint(pointMouse);
    GetClassName(hWnd, lpszClassName, sizeof(lpszClassName));
    if (0 != strcmp(lpszClassName, "Internet Explorer_Server"))
        return CallNextHookEx(_hKeyBoardHook, nCode, wParam, lParam);

    if (nCode < 0)
        return CallNextHookEx(_hKeyBoardHook, nCode, wParam, lParam);


    if ((WM_RBUTTONDOWN == wParam) && (HC_ACTION == nCode))
    {
        CallNextHookEx(_hMouseHook, nCode, wParam, lParam);
        return 1;

        /*        HWND hWnd;
                hWnd = GetActiveWindow();
                if (NULL != hWnd)
                {
                    hWnd = GetTopLevelHWND(hWnd);
                    if (hWnd == _hWndMain)
                    {
                        CallNextHookEx(_hMouseHook, nCode, wParam, lParam);
                        return 1;
                    }
                }
        */

        /*        POINT pt;
                GetCursorPos(&pt);
                HWND hWnd = ChildWindowFromPoint(GetDesktopWindow(), pt);
                if (NULL != hWnd)
                {
                    char strClassName[1024];

                    strClassName[0] = '\0';
                    GetClassName(hWnd, strClassName, sizeof(strClassName));
                    if (0 == strcmp(strClassName, "#32771"))
                    {
                        CallNextHookEx(_hMouseHook, nCode, wParam, lParam);
                        return 1;
                    }
                }
        */
    }

    return CallNextHookEx(_hMouseHook, nCode, wParam, lParam);
}

static LRESULT CALLBACK KeyBoardProc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
    )
{
    bool bFilter = false;
    bool bIsIEControl = false;
    char lpszClassName[1024];
    //HWND hwndFocus=GetFocus();

    POINT pointMouse;
    GetCursorPos(&pointMouse);
    HWND hWnd = ::WindowFromPoint(pointMouse);
    GetClassName(hWnd, lpszClassName, sizeof(lpszClassName));
    if (0 != strcmp(lpszClassName, "Internet Explorer_Server"))
        return CallNextHookEx(_hKeyBoardHook, nCode, wParam, lParam);

    if (nCode < 0)
        return CallNextHookEx(_hKeyBoardHook, nCode, wParam, lParam);
    while (1)
    {
        //filter backspace

        if (0x08 == wParam && true == bIsIEControl && _hWndMain == ::GetActiveWindow())
        {
            bFilter = true;
            break;
        }
        //filter ctrl+n
        int nState = (::GetKeyState(VK_CONTROL) < 0) ? 1 : 0;
        if (nState && (0x4E == wParam))
        {
            bFilter = true;
            break;
        }
        //ctrl+p
        if (nState && (0x50 == wParam))
        {
            bFilter = true;
            break;
        }
        //ctrl+f
        if (nState && (0x46 == wParam))
        {
            bFilter = true;
            break;
        }
        if (nState && (0x4C == wParam))
        {
            bFilter = true;
            break;
        }
        //shift+F10

        nState = (::GetKeyState(VK_SHIFT) < 0) ? 1 : 0;
        if (nState && (VK_F10 == wParam))
        {
            bFilter = true;
            break;
        }
        //get it,but no use
        if (wParam == 0x5D)
        {
            bFilter = true;
            break;
        }

        break;
    }

    //debug
    //bFilter=false;
    if (true == bFilter)
    {
        CallNextHookEx(_hKeyBoardHook, nCode, wParam, lParam);
        return 1;
    }
    else
        return CallNextHookEx(_hKeyBoardHook, nCode, wParam, lParam);
}
