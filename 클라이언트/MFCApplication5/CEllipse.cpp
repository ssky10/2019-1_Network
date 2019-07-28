#include "pch.h"
#include "CEllipse.h"


CEllipse::CEllipse(CPoint _xy1, COLORREF _color, BOOL _isFill) {
	xy1 = xy2 = _xy1;
	mColor = _color;
	is_Fill = _isFill;
	type = ELLIPSE;
}

CEllipse::CEllipse(CString str) {
	CString tmp, tmp2;
	//타입|채우기|색상|xy1|xy2
	AfxExtractSubString(tmp, str, 0, '|'); type = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 1, '|'); ID = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 2, '|'); is_Fill = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 3, '|'); mColor = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 4, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); xy1.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); xy1.y = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 5, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); xy2.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); xy2.y = StrToInt(tmp2);
}

CEllipse::~CEllipse() {

}

void CEllipse::DrawItem(CDC* p) {
	p->Ellipse(xy1.x, xy1.y, xy2.x, xy2.y);
}

BOOL CEllipse::CheckItem(CPoint point) {
	int err = 20;
	double a = (xy2.x - xy1.x) / 2;
	double b = (xy1.y - xy2.y) / 2;
	CPoint tmp;

	if (xy1.x > xy2.x) { tmp = xy2; xy2 = xy1; xy1 = tmp; }

	double result;
	if (xy1.y > xy2.y) result = sqrt(pow(b, 2) * pow(point.x - (a + xy1.x), 2) + pow(a, 2) * pow(point.y - (b + xy2.y), 2)) - a * b;
	else result = sqrt(pow(b, 2) * pow(point.x - (a + xy1.x), 2) + pow(a, 2) * pow(point.y - (-b + xy1.y), 2)) + a * b;

	result = sqrt(fabs(result));

	if (result < err) return true;
	else return false;
}

TCHAR* CEllipse::DataToChar() {
	CString* str = new CString("");

	//타입|채우기|색상|xy1|xy2
	str->Format(_T("%d%d|%d|%d|%ld|(%d,%d)|(%d,%d)"), 1, type, ID, is_Fill, mColor, xy1.x, xy1.y, xy2.x, xy2.y);

	return (TCHAR*)(LPCTSTR)(*str);
}

