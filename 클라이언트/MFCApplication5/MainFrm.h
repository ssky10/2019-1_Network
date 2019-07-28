
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter;
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnClientupdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateItem(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLine();
	afx_msg void OnFree();
	afx_msg void OnEraser();
	afx_msg void OnEllipse();
	afx_msg void OnRect();
	afx_msg void OnColor();
	afx_msg void OnText();
	afx_msg void OnImage();
	afx_msg void OnUpdateEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEraser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFree(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImage(CCmdUI* pCmdUI);
};


