// CLoginDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication5.h"
#include "MFCApplication5Doc.h"
#include "MainFrm.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include <afxsock.h>
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")


// CLoginDlg 대화 상자

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, vPort(_T(""))
{
	
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, mIPAddress);
	//  DDX_Text(pDX, IDC_PORTNUM, vPort);
	DDX_Control(pDX, IDC_PORTNUM, mPort);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
	mPort.GetWindowTextW(vPort);
	if (pDoc->m_ListenSocket.Create(StrToInt(vPort), SOCK_STREAM)) { //소켓 생성
		if (!pDoc->m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() retrun False"));
		}
	}
	else {
		AfxMessageBox(_T("ERROR:Failed to create server socket!"));
	}
	CDialogEx::OnOK();
}

CString CLoginDlg::GetIpAddress()
{
	DWORD Result;
	ULONG BufferLength = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddresses = (PIP_ADAPTER_ADDRESSES)HeapAlloc(GetProcessHeap(), 0, BufferLength);
	//PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	char buf[32] = { 0, };

	//오버 플로우가 발생한 경우 
	Result = GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &BufferLength);
	if (Result == ERROR_BUFFER_OVERFLOW)
	{
		HeapFree(GetProcessHeap(), 0, pAddresses);
		pAddresses = (PIP_ADAPTER_ADDRESSES)HeapAlloc(GetProcessHeap(), 0, BufferLength);
	}


	Result = GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &BufferLength);
	if (Result == NO_ERROR)
	{
		 struct sockaddr_in* pAddr = (struct sockaddr_in*)pAddresses->FirstUnicastAddress->Address.lpSockaddr;
		 inet_ntop(AF_INET,&(pAddr->sin_addr),buf,sizeof(buf));
	}
	else {
		HeapFree(GetProcessHeap(), 0, pAddresses);
	}
	HeapFree(GetProcessHeap(), 0, pAddresses);
	return CString(buf);
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString ip[4];
	vIP = GetIpAddress();
	AfxExtractSubString(ip[0], vIP, 0, '.');
	AfxExtractSubString(ip[1], vIP, 1, '.');
	AfxExtractSubString(ip[2], vIP, 2, '.');
	AfxExtractSubString(ip[3], vIP, 3, '.');
	mIPAddress.SetAddress(StrToInt(ip[0]), StrToInt(ip[1]), StrToInt(ip[2]), StrToInt(ip[3]));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
