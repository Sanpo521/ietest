#include <atlbase.h>
#include "UnitConversionUtil.h"

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :   
return value		:	转换后毫米值
******************************************************************************************/
float Inch2Millimeter(float inch) {
	float result = inch * 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :
return value		:	转换后毫米值
******************************************************************************************/
float Inch2Millimeter(TCHAR* inch) {
	float result = _ttof(inch) * 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :
return value		:	转换后毫米值
******************************************************************************************/
BOOL Inch2Millimeter(float inch, TCHAR* millimeter) {
	float result = inch * 25.4;
	_stprintf_s(millimeter, MAX_PATH, _T("%f"), result);
	return TRUE;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :
return value		:	转换后毫米值
******************************************************************************************/
BOOL Inch2Millimeter(TCHAR* inch, TCHAR* millimeter) {
	float result = _ttof(inch) * 25.4;
	_stprintf_s(millimeter, MAX_PATH, _T("%f"), result);
	return TRUE;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将毫米转换为英寸(25.4毫米=1英寸)
Input parameters    :	毫米值
Output parameters   :
return value		:	转换后英寸值
******************************************************************************************/
float Millimeter2Inch(float millimeter) {
	float result = millimeter / 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :
return value		:	转换后毫米值
******************************************************************************************/
float Millimeter2Inch(TCHAR* millimeter) {
	float result = _ttof(millimeter) / 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :
return value		:	转换后毫米值
******************************************************************************************/
BOOL Millimeter2Inch(float millimeter, TCHAR* inch) {
	float result = millimeter / 25.4;
	_stprintf_s(inch, MAX_PATH, _T("%f"), result);
	return TRUE;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	将英寸转换为毫米(1英寸=25.4毫米)
Input parameters    :	英寸值
Output parameters   :
return value		:	转换后毫米值
******************************************************************************************/
BOOL Millimeter2Inch(TCHAR* millimeter, TCHAR* inch) {
	float result = _ttof(millimeter) / 25.4;
	_stprintf_s(inch, MAX_PATH, _T("%f"), result);
	return TRUE;
}

///******************************************************************************************
//Name	            :	Inch2Millimeter
//Date	            :	2020-05-25
//Description         :	将英寸转换为毫米(1英寸=25.4毫米)
//Input parameters    :	英寸值
//Output parameters   :
//return value		:	转换后毫米值
//******************************************************************************************/
//BOOL Millimeter2Inch(TCHAR* millimeter, TCHAR* inch) {
//	float result = _ttof(inch) / 25.4;
//	_stprintf_s(inch, MAX_PATH, _T("%f"), result);
//	return TRUE;
//}