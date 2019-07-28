#pragma once
#include "Item.h"
class CRectangle :
	public Item
{
public:
	CRectangle(CString str);
	CRectangle(CPoint _xy1, COLORREF _color, BOOL _isFill);
	CRectangle(CPoint _xy1, CPoint _xy2, COLORREF _color, BOOL _isFill);
	~CRectangle();
	void DrawItem(CDC* p);
	BOOL CheckItem(CPoint point);
	void SetXY2(CPoint _xy2) { xy2 = _xy2; }
	TCHAR* DataToChar();

	CPoint xy1, xy2;
};

