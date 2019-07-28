#include "pch.h"
#include "CText.h"
#include "CTextDlg.h"
extern CString pFont[4];
CText::CText(CString str) {
	CString tmp, tmp2;
	//타입|채우기|색상|xy|size|font||context
	AfxExtractSubString(tmp, str, 0, '|'); type = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 1, '|'); ID = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 2, '|'); is_Fill = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 3, '|'); mColor = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 4, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); xy.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); xy.y = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 5, '|'); size = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 6, '|'); font = StrToInt(tmp);
	int idx = str.Find(_T("||"), 0);
	context = str.Mid(idx + 2);
}
CText::CText(CPoint _xy, CString _context, int _size, int _font, COLORREF _color) {
	xy = _xy; context = _context; size = _size; font = _font; mColor = _color;
	type = this->TEXT;
}
CText::~CText() {

}
void CText::DrawItem(CDC * p) {
	CFont f;
	//f.CreatePointFont(t.Cb.GetCurSel() + 1, pFont[t.Lb.GetCurSel()]);
	f.CreatePointFont((size) * 10, pFont[font]);
	
	p->SelectObject(&f);
	p->TextOut(xy.x, xy.y, context);
}
BOOL CText::CheckItem(CPoint point) {
	if ((point.x < ((xy.x) + (context.GetLength() * size * 2))) && (point.x > (xy.x)))
		if ((point.y < ((xy.y) + (size * 10))) && (point.y > (xy.y)))
			return true;
	return false;
}
TCHAR* CText::DataToChar() {
	CString* str = new CString("");

	//타입|채우기|색상|xy|size|font||context
	str->Format(_T("%d%d|%d|%d|%ld|(%d,%d)|%d|%d||%s"), 1, type, ID, is_Fill, mColor, xy.x, xy.y, size, font, (LPCTSTR)context);

	return (TCHAR*)(LPCTSTR)(*str);
}