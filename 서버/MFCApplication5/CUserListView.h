#pragma once
#include "resource.h"

// MainView �� ���Դϴ�.

class CUserListView : public CFormView
{
	DECLARE_DYNCREATE(CUserListView)

protected:
	CUserListView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CUserListView();

public:
	enum { IDD = IDD_USERLIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox connectedList;
};