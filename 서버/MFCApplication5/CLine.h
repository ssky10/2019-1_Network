#pragma once
#include "Item.h"
class CLine :
	public Item
{
public:
	CLine(CString str);
	CLine(CPoint _xy1, COLORREF _color);
	~CLine();
	void DrawItem(CDC* p);
	BOOL CheckItem(CPoint point);
	void SetXY2(CPoint _xy2) { xy2 = _xy2; }
	TCHAR* DataToChar();

	CPoint xy1, xy2;
};

