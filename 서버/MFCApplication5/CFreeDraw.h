#pragma once
#include <vector>
#include "Item.h"

class CFreeDraw :
	public Item
{
public:
	CFreeDraw(CString str);
	CFreeDraw(CPoint _xy1, COLORREF _color);
	~CFreeDraw();
	BOOL AddPoint(CPoint xy);
	void DrawItem(CDC* p);
	BOOL CheckItem(CPoint point);
	TCHAR* DataToChar();

	std::vector<CPoint> points;
	CPoint LTxy, RBxy; //������� ��ǥ, �����ϴ� ��ǥ
};

