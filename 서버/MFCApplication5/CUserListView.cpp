#include "pch.h"
#include "CUserListView.h"

// CUserListView

IMPLEMENT_DYNCREATE(CUserListView, CFormView)

CUserListView::CUserListView()
	: CFormView(CUserListView::IDD)
{

}

CUserListView::~CUserListView()
{
}

void CUserListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONNECTED_LIST, connectedList);
}

BEGIN_MESSAGE_MAP(CUserListView, CFormView)
END_MESSAGE_MAP()


// MainView 진단입니다.

#ifdef _DEBUG
void CUserListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CUserListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MainView 메시지 처리기입니다.
