﻿
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
#include "CClientSocket.h"

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
}

CMFCApplication5Doc::~CMFCApplication5Doc()
{
	POSITION pos;

	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	//생성되어있는 클라이언트 소켓이 없을때까지 체크하여 소켓닫기
	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->ShutDown(); //연결된 상대방 소켓에 연결이 종료됨을 알린다.
			pClient->Close(); //소켓을 닫는다.

			delete pClient;
		}
		m_ListenSocket.ShutDown();
		m_ListenSocket.Close();
	}
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
void CMFCApplication5Doc::addClient(CString user) {

}

void CMFCApplication5Doc::addItem(Item* item) {
	item->setID(ItemID++);
	list.push_back(item);
	m_ListenSocket.SendAllMessage(item->DataToChar());
}

void CMFCApplication5Doc::deleteItem(int idx) {
	m_ListenSocket.SendDeleteItem(list[idx]->ID);
	delete list[idx];
	list.erase(list.begin() + idx);
}