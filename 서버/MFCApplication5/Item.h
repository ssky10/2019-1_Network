#pragma once
class Item
{
public:
	static const int LINE = 1;
	static const int ELLIPSE = 2;
	static const int RECTANGLE = 3;
	static const int FREEDRAW = 4;
	static const int TEXT = 5;
	static const int IMAGE = 6;
	static const int ERASER = 10;

public:
	virtual ~Item(){};
	void Draw(CDC* p, BOOL isReDraw = false) {
		CBrush br;
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, mColor);
		br.CreateSolidBrush(mColor);
		p->SelectObject(&pen);
		p->SelectObject(&br);
		p->SetTextColor(mColor);

		if (isReDraw) {
			p->SetROP2(R2_NOTXORPEN);
		}		
		DrawItem(p);
	}
	virtual void DrawItem(CDC* p) = 0;
	virtual BOOL CheckItem(CPoint point) = 0;
	virtual TCHAR* DataToChar() { return _T(""); };
	virtual void CharToData(CString str) {};
	int GetType() {
		return type;
	}
	void setID(int id) {
		ID = id;
	}

public:
	int ID;
	int type = -1;
	BOOL is_Fill;
	COLORREF mColor;
};