#include "pch.h"
#include "CImageItem.h"
#include <vector>
#include <string>

CImageItem::~CImageItem()
{
	CFileFind file;
	if (file.FindFile(path + name)) {
		DeleteFile(path + name);
	}
	delete img, path, name;
}

CImageItem::CImageItem(CPoint _xy, CString _name) {
	xy = _xy; name = _name; type = IMAGE;
	HRESULT hResult = img.Load(path + name);
	if (FAILED(hResult)) {
		AfxMessageBox(_T("이미지를 가져오는데 실패했습니다."));\
		return;
	}
	size.cx = img.GetWidth();
	size.cy = img.GetHeight();
}

CImageItem::CImageItem(TCHAR* buf) {
	CString str(buf);
	CString tmp, tmp2;
	int imgLen;
	//타입|채우기|색상|xy|size|name|imgLen||img
	AfxExtractSubString(tmp, str, 0, '|'); type = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 1, '|'); ID = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 2, '|'); is_Fill = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 3, '|'); mColor = StrToInt(tmp);
	AfxExtractSubString(tmp, str, 4, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); xy.x = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); xy.y = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 5, '|'); tmp.Replace(_T("("), _T("")); tmp.Replace(_T(")"), _T("")); AfxExtractSubString(tmp2, tmp, 0, ','); size.cx = StrToInt(tmp2); AfxExtractSubString(tmp2, tmp, 1, ','); size.cy = StrToInt(tmp2);
	AfxExtractSubString(tmp, str, 6, '|'); name = tmp;
	AfxExtractSubString(tmp, str, 7, '|'); imgLen = StrToInt(tmp);
	int idx = str.Find(_T("||"), 0);
	//TCHAR *stream = new TCHAR[imgLen];
	char* bitmapBuffer = new char[imgLen];
	int readSize = imgLen;
	char* pBuffer = bitmapBuffer;
	TCHAR* start = buf + (idx + 2);
	memcpy(pBuffer, start, imgLen * sizeof(char));
	
	
	IStream * strm = SHCreateMemStream((BYTE*)pBuffer, imgLen * sizeof(char));
	if (strm)
	{
		img.Load(strm);
		InitPath();
		img.Save(path + name, Gdiplus::ImageFormatJPEG);
		strm->Release();
	}
	else {
		AfxMessageBox(_T("이미지를 가져오는데 실패했습니다."));
	}
}

void CImageItem::DrawItem(CDC * p) {
	img.BitBlt(p->m_hDC, xy.x, xy.y);
}

BOOL CImageItem::CheckItem(CPoint point) {
	if (img.IsNull()) return true;
	CRect r(xy, size);
	if (r.PtInRect(point)) return true;
	return false;
}

TCHAR* CImageItem::DataToChar() {
	CString* str = new CString("");

	//타입|채우기|색상|xy|size|name||img
	str->Format(_T("%d|%d|%ld|(%d,%d)|(%d,%d)|%s||"), type, is_Fill, mColor, xy.x, xy.y, size.cx, size.cy, (LPCTSTR)name);
	IStream* stream = NULL;
	std::vector<BYTE> buf;
	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
	if (!SUCCEEDED(hr))
		str->Format(_T("%d|%d|%ld|(%d,%d)|%d|%d||%s"), this->TEXT, is_Fill, mColor, xy.x, xy.y, 10, 0, (LPCTSTR)(name + "(전송실패)"));
	else {
		img.Save(stream, Gdiplus::ImageFormatBMP);
		ULARGE_INTEGER liSize;
		IStream_Size(stream, &liSize);
		DWORD len = liSize.LowPart;
		IStream_Reset(stream);
		buf.resize(len);
		IStream_Read(stream, &buf[0], len);
		std::string s(buf.begin(), buf.end());
		str->Append(CString::CStringT(CA2CT(s.c_str())));
		stream->Release();
	}
	return (TCHAR*)(LPCTSTR)(*str);
}

const CString CImageItem::InitPath() {
	CFileFind file;
	if (!file.FindFile(_T("C:\\Temp*.*"))) CreateDirectoryW(_T("C:\\Temp\\"), NULL);
	if (!file.FindFile(_T("C:\\Temp\\NetPaint*.*"))) CreateDirectoryW(_T("C:\\Temp\\NetPaint\\"), NULL);
	if (!file.FindFile(_T("C:\\Temp\\NetPaint\\Client*.*"))) CreateDirectoryW(_T("C:\\Temp\\NetPaint\\Client\\"), NULL);
	return path;
}

const CString CImageItem::path = CString("C:\\Temp\\NetPaint\\Client\\");;