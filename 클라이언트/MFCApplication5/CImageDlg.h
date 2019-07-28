#pragma once


// CImageDlg 대화 상자

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImageDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedImgOpenBtn();
	CEdit imgPathEdit;
	CString height;
	CString width;
	afx_msg void OnBnClickedOk();
	CString imgPath;
};
