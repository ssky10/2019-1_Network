#pragma once
#include "Item.h"
class CText :
	public Item
{
public:
	CText(CString str);
	CText(CPoint _xy, CString _context, int _size, int _font, COLORREF _color);
	~CText();
	void DrawItem(CDC* p);
	BOOL CheckItem(CPoint point);
	TCHAR* DataToChar();


	CPoint xy;
	CString context;
	int size, font;
};

