#pragma once
//IE页面打印设置串最大长度
#define MAX_IEPS				1000

BOOL GetIEPageSetup(TCHAR* pPageSetup);
BOOL SetIEPageSetup(TCHAR* pPageSetup);