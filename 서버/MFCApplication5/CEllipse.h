#pragma once
#include "Item.h"
class CEllipse :
	public Item
{
public:
	CEllipse(CString str);
	CEllipse(CPoint _xy1, COLORREF _color, BOOL _isFill);
	~CEllipse();
	void DrawItem(CDC* p);
	BOOL CheckItem(CPoint point);
	void SetXY2(CPoint _xy2) { xy2 = _xy2; }
	TCHAR* DataToChar();

	CPoint xy1, xy2;
};

