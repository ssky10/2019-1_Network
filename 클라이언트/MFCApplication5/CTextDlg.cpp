// CTextDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication5.h"
#include "CTextDlg.h"
#include "afxdialogex.h"

CString pFont[4] = { L"명조체",L"Arial",L"Hobo BT" ,L"Sans Serif" };

// CTextDlg 대화 상자

IMPLEMENT_DYNAMIC(CTextDlg, CDialogEx)

CTextDlg::CTextDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEXT, pParent)
	, mText(_T(""))
{

}

CTextDlg::~CTextDlg()
{
}

void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT, mText);
	DDX_Control(pDX, IDC_TYPE, Lb);
	DDX_Control(pDX, IDC_SIZE, Cb);
}


BEGIN_MESSAGE_MAP(CTextDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTextDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTextDlg 메시지 처리기
BOOL CTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString s;
	for (int i = 0; i < 50; i++) {
		s.Format(L"%d", i + 1);
		Cb.InsertString(i, s);
	}
	for (int i = 0; i < 4; i++)
		Lb.InsertString(i, pFont[i]);


	Cb.SetCurSel(10);
	Lb.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTextDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	a = Cb.GetCurSel();
	b = Lb.GetCurSel();
	CDialogEx::OnOK();
}
