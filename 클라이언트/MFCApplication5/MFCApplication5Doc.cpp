
// MFCApplication5Doc.cpp: CMFCApplication5Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication5.h"
#endif

#include "MFCApplication5Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication5Doc

IMPLEMENT_DYNCREATE(CMFCApplication5Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication5Doc, CDocument)
END_MESSAGE_MAP()


// CMFCApplication5Doc 생성/소멸

CMFCApplication5Doc::CMFCApplication5Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	list.clear();
	type = 1;
	m_Socket.Create();
	if (m_Socket.Connect(_T("127.0.0.1"), 21000) == FALSE) {
		AfxMessageBox(_T("ERROR:Failed to connect Server"));
		//PostQuitMessage(0);
		//return FALSE;
	}
}

CMFCApplication5Doc::~CMFCApplication5Doc()
{
}

BOOL CMFCApplication5Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMFCApplication5Doc serialization

void CMFCApplication5Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMFCApplication5Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMFCApplication5Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCApplication5Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplication5Doc 진단

#ifdef _DEBUG
void CMFCApplication5Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication5Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCApplication5Doc 명령

void CMFCApplication5Doc::addItem(Item* item) {
	TCHAR* pszMessage = item->DataToChar();
	if (m_Socket != NULL) {
		//send 함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
		//이 함수는 전송한 데이터의 길이를 반환한다.
		int checkLenOfData = m_Socket.Send(pszMessage, (lstrlen(pszMessage) + 1) * 2);
		if (checkLenOfData != (lstrlen(pszMessage) + 1) * 2) {
			AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다."));
		}
	}
}

void CMFCApplication5Doc::deleteItem(int idx) {
	CString msg;
	msg.Format(_T("%d%d"), 2, list[idx]->ID);

	if (m_Socket != NULL) {
		//send 함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
		//이 함수는 전송한 데이터의 길이를 반환한다.
		int checkLenOfData = m_Socket.Send((TCHAR*)(LPCTSTR)(msg), (msg.GetLength() + 1) * 2);
		if (checkLenOfData != (msg.GetLength() + 1) * 2) {
			AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다."));
		}
	}
	delete list[idx];
	list.erase(list.begin() + idx);
}
