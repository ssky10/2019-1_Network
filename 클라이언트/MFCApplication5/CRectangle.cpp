#include "pch.h"
#include "CRectangle.h"

CRectangle::CRectangle(CPoint _xy1, COLORREF _color, BOOL _isFill) {
	xy1 = xy2 = _xy1;
	mColor = _color;
	is_Fill = _isFill;
	type = RECTANGLE;
}

CRectangle::CRectangle(CPoint _xy1, CPoint _xy2, COLORREF _color, BOOL _isFill) {
	xy1 = _xy1; xy2 = _xy2;
	mColor = _color;
	is_Fill = _isFill;
}

CRectangle::CRectangle(CString str) {
	CString tmp, tmp2;
	//타입|채우기|색상|xy1|xy2
	AfxExtractSubString(tmp, str, 0, '|'); type = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 1, '|'); ID = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 2, '|'); is_Fill = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 3, '|'); mColor = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 4, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); xy1.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); xy1.y = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 5, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); xy2.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); xy2.y = StrToInt(tmp2);
}

CRectangle::~CRectangle() {

}

void CRectangle::DrawItem(CDC* p) {
	p->Rectangle(xy1.x, xy1.y, xy2.x, xy2.y);
}

BOOL CRectangle::CheckItem(CPoint point) {
	int err = 10;
	CPoint tmp;

	if (xy1.x > xy2.x) { tmp = xy2; xy2 = xy1; xy1 = tmp; }
	if ((abs(xy1.x - point.x) <= err) || (abs(xy2.x - point.x) <= err)) return true;
	else if ((abs(xy1.y - point.y) <= err) || (abs(xy2.y - point.y) <= err)) return true;
	else return false;
}


TCHAR* CRectangle::DataToChar() {
	CString* str = new CString("");

	//타입|채우기|색상|xy1|xy2
	str->Format(_T("%d%d|%d|%d|%ld|(%d,%d)|(%d,%d)"), 1, type, ID, is_Fill, mColor, xy1.x, xy1.y, xy2.x, xy2.y);

	return (TCHAR*)(LPCTSTR)(*str);
}

