
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication5.h"

#include "MainFrm.h"
#include "CMainView.h"
#include "CUserListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_CLIENTUPDATE, &CMainFrame::OnClientupdate)
	ON_MESSAGE(UM_UPDATEITEM, &CMainFrame::OnUpdateItem)
	ON_COMMAND(IDM_LINE, &CMainFrame::OnLine)
	ON_COMMAND(IDM_FREE, &CMainFrame::OnFree)
	ON_COMMAND(IDM_ERASER, &CMainFrame::OnEraser)
	ON_COMMAND(IDM_ELLIPSE, &CMainFrame::OnEllipse)
	ON_COMMAND(IDM_RECT, &CMainFrame::OnRect)
	ON_UPDATE_COMMAND_UI(IDM_ELLIPSE, &CMainFrame::OnUpdateEllipse)
	ON_UPDATE_COMMAND_UI(IDM_ERASER, &CMainFrame::OnUpdateEraser)
	ON_UPDATE_COMMAND_UI(IDM_FREE, &CMainFrame::OnUpdateFree)
	ON_UPDATE_COMMAND_UI(IDM_LINE, &CMainFrame::OnUpdateLine)
	ON_UPDATE_COMMAND_UI(IDM_RECT, &CMainFrame::OnUpdateRect)
	ON_COMMAND(IDM_COLOR, &CMainFrame::OnColor)
	ON_UPDATE_COMMAND_UI(IDM_COLOR, &CMainFrame::OnUpdateColor)
	ON_COMMAND(IDM_TEXT, &CMainFrame::OnText)
	ON_UPDATE_COMMAND_UI(IDM_TEXT, &CMainFrame::OnUpdateText)
	ON_COMMAND(IDM_IMAGE, &CMainFrame::OnImage)
	ON_UPDATE_COMMAND_UI(IDM_IMAGE, &CMainFrame::OnUpdateImage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	SetMenu(NULL); //메뉴바 제거

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	CRect rect;
	GetClientRect(rect);
	CSize size1(MulDiv(rect.Width(), 100, 100), ::GetSystemMetrics(SM_CYSCREEN)); // 폭(80%)
	CSize size2(MulDiv(rect.Width(), 0, 100), ::GetSystemMetrics(SM_CYSCREEN)); // 폭(20%)
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2)) { 
		TRACE0("Failed to CreateStatic Splitter \n"); 
		return FALSE; 
	}
	
	// 사용자가 만든 CFormView1 을 우측에 배치합니다... 
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMainView), size1, pContext)){ 
		TRACE0("Failed to create CFormView1 pane \n");
		return FALSE;
	} 
	// 사용자가 만든 CFormView2 을 좌측에 배치합니다... 
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CUserListView), size2, pContext)) { 
		TRACE0("Failed to create CFormView2 pane \n");
		return FALSE; 
	} 
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
afx_msg LRESULT CMainFrame::OnClientupdate(WPARAM wParam, LPARAM lParam)
{
	CUserListView* listview = (CUserListView*)m_wndSplitter.GetPane(0, 0);
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	listview->connectedList.ResetContent();
	POSITION pos = pDoc->clientList.GetHeadPosition();
	while (pos != NULL) {
		CString string = pDoc->clientList.GetNext(pos);
		listview->connectedList.AddString(string);
	}
	return 0;
}


afx_msg LRESULT CMainFrame::OnUpdateItem(WPARAM wParam, LPARAM lParam)
{
	CMainView* mainView = (CMainView*)m_wndSplitter.GetPane(0, 1);
	mainView->Invalidate();
	return 0;
}


void CMainFrame::OnColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog c;
	if (c.DoModal() == IDOK) {
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
		pDoc->mColor = c.GetColor();
	}
}


void CMainFrame::OnUpdateColor(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

}


void CMainFrame::OnLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::LINE;
}


void CMainFrame::OnFree()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::FREEDRAW;
}


void CMainFrame::OnEraser()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::ERASER;
}


void CMainFrame::OnEllipse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::ELLIPSE;
}


void CMainFrame::OnRect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::RECTANGLE;
}

void CMainFrame::OnText()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::TEXT;
}

void CMainFrame::OnImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pDoc->type = Item::IMAGE;
}

void CMainFrame::OnUpdateEllipse(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::ELLIPSE);
}


void CMainFrame::OnUpdateEraser(CCmdUI * pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::ERASER);
}


void CMainFrame::OnUpdateFree(CCmdUI * pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::FREEDRAW);
}


void CMainFrame::OnUpdateLine(CCmdUI * pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::LINE);
}


void CMainFrame::OnUpdateRect(CCmdUI * pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::RECTANGLE);
}



void CMainFrame::OnUpdateText(CCmdUI * pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::TEXT);
}


void CMainFrame::OnUpdateImage(CCmdUI * pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->type == Item::IMAGE);
}

