#include <atlbase.h>
#include "UnitConversionUtil.h"

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :   
return value		:	ת�������ֵ
******************************************************************************************/
float Inch2Millimeter(float inch) {
	float result = inch * 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :
return value		:	ת�������ֵ
******************************************************************************************/
float Inch2Millimeter(TCHAR* inch) {
	float result = _ttof(inch) * 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :
return value		:	ת�������ֵ
******************************************************************************************/
BOOL Inch2Millimeter(float inch, TCHAR* millimeter) {
	float result = inch * 25.4;
	_stprintf_s(millimeter, MAX_PATH, _T("%f"), result);
	return TRUE;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :
return value		:	ת�������ֵ
******************************************************************************************/
BOOL Inch2Millimeter(TCHAR* inch, TCHAR* millimeter) {
	float result = _ttof(inch) * 25.4;
	_stprintf_s(millimeter, MAX_PATH, _T("%f"), result);
	return TRUE;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	������ת��ΪӢ��(25.4����=1Ӣ��)
Input parameters    :	����ֵ
Output parameters   :
return value		:	ת����Ӣ��ֵ
******************************************************************************************/
float Millimeter2Inch(float millimeter) {
	float result = millimeter / 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :
return value		:	ת�������ֵ
******************************************************************************************/
float Millimeter2Inch(TCHAR* millimeter) {
	float result = _ttof(millimeter) / 25.4;
	return result;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :
return value		:	ת�������ֵ
******************************************************************************************/
BOOL Millimeter2Inch(float millimeter, TCHAR* inch) {
	float result = millimeter / 25.4;
	_stprintf_s(inch, MAX_PATH, _T("%f"), result);
	return TRUE;
}

/******************************************************************************************
Name	            :	Inch2Millimeter
Date	            :	2020-05-25
Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
Input parameters    :	Ӣ��ֵ
Output parameters   :
return value		:	ת�������ֵ
******************************************************************************************/
BOOL Millimeter2Inch(TCHAR* millimeter, TCHAR* inch) {
	float result = _ttof(millimeter) / 25.4;
	_stprintf_s(inch, MAX_PATH, _T("%f"), result);
	return TRUE;
}

///******************************************************************************************
//Name	            :	Inch2Millimeter
//Date	            :	2020-05-25
//Description         :	��Ӣ��ת��Ϊ����(1Ӣ��=25.4����)
//Input parameters    :	Ӣ��ֵ
//Output parameters   :
//return value		:	ת�������ֵ
//******************************************************************************************/
//BOOL Millimeter2Inch(TCHAR* millimeter, TCHAR* inch) {
//	float result = _ttof(inch) / 25.4;
//	_stprintf_s(inch, MAX_PATH, _T("%f"), result);
//	return TRUE;
//}