// CMainView.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication5.h"
#include "MFCApplication5Doc.h"
#include "CMainView.h"
#include "CTextDlg.h"
#include "CFreeDraw.h"
#include "CLine.h"
#include "CEllipse.h"
#include "CRectangle.h"
#include "CText.h"
#include "CImageItem.h"
#include "CImageDlg.h"
#include <atlimage.h>

extern CString pFont[4];
// CMainView


IMPLEMENT_DYNCREATE(CMainView, CView)

CMainView::CMainView()
{
	tmp = NULL;
}

CMainView::~CMainView()
{
}

BEGIN_MESSAGE_MAP(CMainView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMainView 그리기

void CMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CBrush br;
	br.CreateStockObject(NULL_BRUSH);
	pDC->SelectObject(&br);
	DrawPT(pDC);
}


// CMainView 진단

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainView 메시지 처리기

void CMainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetDocument();
	COLORREF mColor = pDoc->mColor;

	switch (pDoc->type)
	{
	case Item::LINE:
		tmp = new CLine(point, mColor);
		break;
	case Item::ELLIPSE:
		tmp = new CEllipse(point, mColor, true);
		break;
	case Item::RECTANGLE:
		tmp = new CRectangle(point, mColor, true);
		break;
	case Item::FREEDRAW:
		tmp = new CFreeDraw(point, mColor);
		break;
	case Item::ERASER:
		OnRButtonDown(nFlags, point);
		break;
	case Item::TEXT:
	{
		CTextDlg t;

		if (t.DoModal() == IDOK) {
			CMFCApplication5Doc* doc = (CMFCApplication5Doc*)GetDocument();
			doc->list.push_back(new CText(point, t.mText, t.a, t.b, mColor));
			doc->addItem((doc->list)[doc->list.size() - 1]);
			Invalidate();
		}
		break;
	}
	case Item::IMAGE:
	{
		CImageDlg i;

		if (i.DoModal() == IDOK) {
			CString path = i.imgPath;
			path.Replace(_T("\\"), _T("\\\\"));
			CImage src, img;
			HRESULT hResult = src.Load(path);
			if (FAILED(hResult)) {
				AfxMessageBox(_T("이미지를 가져오는데 실패했습니다."));
			}
			else {
				int w = src.GetWidth() * (StrToIntW(i.width) / 100.0), h = src.GetHeight() * (StrToIntW(i.height) / 100.0);
				CDC * pSourceDC = CDC::FromHandle(src.GetDC());    // 원본 이미지의 DC를 얻는다
				img.Create(w, h, pSourceDC->GetDeviceCaps(BITSPIXEL));
				HDC h_dc = img.GetDC();
				CTime time = CTime::GetCurrentTime();
				CString path, name;
				path = CImageItem::InitPath();
				name.Format(_T("%04d%02d%02d%02d%02d%02d.jpg"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
				SetStretchBltMode(h_dc, HALFTONE);
				src.StretchBlt(h_dc, 0, 0, w, h, SRCCOPY);
				img.Save(path + name, Gdiplus::ImageFormatJPEG);
				src.ReleaseDC();
				img.ReleaseDC();
				CMFCApplication5Doc * doc = (CMFCApplication5Doc*)GetDocument();
				doc->list.push_back(new CImageItem(point, name));
				Invalidate();
			}
		}
		break;
	}
	default:
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (tmp == NULL) return;
	CMFCApplication5Doc* doc = (CMFCApplication5Doc*)GetDocument();
	doc->addItem(tmp);
	tmp = NULL;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate(); //OnDraw함수 자동실행
	CView::OnLButtonUp(nFlags, point);
}


void CMainView::DrawPT(CDC * p)
{
	CMFCApplication5Doc* doc = (CMFCApplication5Doc*)GetDocument();
	for (int i = 0; i < doc->list.size(); i++) {
		doc->list[i]->Draw(p);
	}
}


void CMainView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMFCApplication5Doc* doc = (CMFCApplication5Doc*)GetDocument();
	for (int i = 0; i < doc->list.size(); i++) {
		if (doc->list[i]->CheckItem(point)) {
			//delete doc->list[i];
			doc->deleteItem(doc->list[i]->ID);
		}
	}
	Invalidate(); //OnDraw함수 자동실행
	CView::OnRButtonDown(nFlags, point);
}

void CMainView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((nFlags & MK_LBUTTON)&&(tmp != NULL)) {
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetDocument();
		int type = pDoc->type;
		CClientDC dc(this);
		tmp->Draw(&dc, true);
		if (type == Item::FREEDRAW) {
			((CFreeDraw*)tmp)->AddPoint(point);
		}
		else if (type == Item::LINE) {
			((CLine*)tmp)->SetXY2(point);
		}
		else if (type == Item::RECTANGLE) {
			((CRectangle*)tmp)->SetXY2(point);
		}
		else if (type == Item::ELLIPSE) {
			((CEllipse*)tmp)->SetXY2(point);
		}
		tmp->Draw(&dc, true);
	}
	CView::OnMouseMove(nFlags, point);
}
