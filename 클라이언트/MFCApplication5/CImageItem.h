#pragma once
#include "Item.h"
#include <atlimage.h>

class CImageItem :
	public Item
{
public:
	~CImageItem();
	CImageItem(TCHAR* buf);
	CImageItem(CPoint _xy, CString name);
	void DrawItem(CDC* p);
	BOOL CheckItem(CPoint point);
	TCHAR* DataToChar();
	static const CString InitPath();


	CPoint xy;
	SIZE size;
	CImage img;
	static const CString path;
	CString name;
};

