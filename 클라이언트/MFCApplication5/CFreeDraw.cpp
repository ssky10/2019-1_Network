#include "pch.h"
#include "CFreeDraw.h"
#include "CRectangle.h"


CFreeDraw::CFreeDraw(CPoint _xy1, COLORREF _color) {
	LTxy = RBxy = _xy1;
	points.push_back(_xy1);
	mColor = _color;
	type = FREEDRAW;
}

CFreeDraw::CFreeDraw(CString str) {
	CString tmp, tmp2;
	int size;
	//타입|채우기|색상|LTxy|RBxy|points사이즈|points...
	AfxExtractSubString(tmp, str, 0, '|'); type = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 1, '|'); ID = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 2, '|'); is_Fill = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 3, '|'); mColor = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 4, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); LTxy.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); LTxy.y = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 5, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); RBxy.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); RBxy.y = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 6, '|'); size = StrToInt(tmp);
	for (int i = 0; i < size; i++)
	{
		int x, y;
		AfxExtractSubString(tmp, str, 6 + i, '|');
		tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T(""));
		AfxExtractSubString(tmp2, tmp, 0, ',');
		x = StrToInt(tmp2);
		AfxExtractSubString(tmp2, tmp, 1, ',');
		y = StrToInt(tmp2);
		points.push_back(CPoint(x, y));
	}
}

CFreeDraw::~CFreeDraw() {

}

BOOL CFreeDraw::AddPoint(CPoint xy) {
	if (points.size() < 100) return FALSE;
	points.push_back(xy);
	if (LTxy.x > xy.x) LTxy.x = xy.x;
	if (LTxy.y > xy.y) LTxy.y = xy.y;
	if (RBxy.x < xy.x) RBxy.x = xy.x;
	if (RBxy.y < xy.y) RBxy.y = xy.y;
	return TRUE;
}

void CFreeDraw::DrawItem(CDC* p) {
	for (int i = 1; i < points.size(); i++) {
		p->MoveTo(points[i - 1]);
		p->LineTo(points[i]);
	}
}

BOOL CFreeDraw::CheckItem(CPoint point) {
	int err = 10;
	CRectangle Boundary(LTxy, RBxy, mColor, false);
	for (int i = 1; i < points.size(); i++) {
		if (Boundary.CheckItem(point)) {
			if (abs(points[i].x - points[i - 1].x) < err) {
				if (points[i - 1].y > points[i].y)
					if ((points[i - 1].y >= point.y) && (points[i].y <= point.y)) return true;
					else return false;
				else
					if ((points[i - 1].y <= point.y) && (points[i].y >= point.y)) return true;
					else return false;
			}
			else {
				double A = (points[i - 1].y - points[i].y) * (point.x - points[i - 1].x) / (points[i - 1].x - points[i].x) - (point.y - points[i - 1].y);
				if (fabs(A) < err) return true;
				else return false;
			}
		}
	}
	return false;
}

TCHAR* CFreeDraw::DataToChar() {
	CString* str = new CString("");

	//타입|채우기|색상|LTxy|RBxy|points사이즈|points...
	str->Format(_T("%d%d|%d|%d|%ld|(%d,%d)|(%d,%d)|%d"), 1, type, ID, is_Fill, mColor, LTxy.x, LTxy.y, RBxy.x, RBxy.y, points.size());

	for each (CPoint var in points)
	{
		str->AppendFormat(_T("|(%d,%d)"), var.x, var.y);
	}

	return (TCHAR*)(LPCTSTR)(*str);
}

