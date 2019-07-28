// CImageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication5.h"
#include "CImageDlg.h"
#include "afxdialogex.h"


// CImageDlg 대화 상자

IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGE, pParent)
	, height(_T("100"))
	, width(_T("100"))
	, imgPath(_T(""))
{

}

CImageDlg::~CImageDlg()
{
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG_EDIT, imgPathEdit);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, height);
	DDX_Text(pDX, IDC_WIDTH_EDIT, width);
	DDX_Text(pDX, IDC_IMG_EDIT, imgPath);
}


BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_IMGOPEN_BTN, &CImageDlg::OnClickedImgOpenBtn)
	ON_BN_CLICKED(IDOK, &CImageDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CImageDlg 메시지 처리기


void CImageDlg::OnClickedImgOpenBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter);

	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		imgPathEdit.SetWindowTextW(pathName);
	}
}


void CImageDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
