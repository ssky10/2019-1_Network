#include "pch.h"
#include "CClientSocket.h"
#include "CListenSocket.h"
#include "MainFrm.h"
#include "MFCApplication5Doc.h"
#include "MFCApplication5View.h"

CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket) {
	m_pListenSocket = pSocket;
}

void CClientSocket::OnClose(int nErrorCode) {
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode) {
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[1024];
	::ZeroMemory(strBuffer, sizeof(strBuffer));

	GetPeerName(strIPAddress, uPortNumber);
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { //전달된 데이터가 있을경우
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
		strTmp.Format(_T("[%s:%d]:%s"), strIPAddress, uPortNumber, strBuffer);
		//pMain->m_List.AddString(strTmp); //메시지 리스트에 입력받은 메시지 띄우기
		//pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);
		
		CListenSocket * pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->SendAllMessage(strBuffer); //다른 클라이언트들에게 메시지 전달
	}

	CSocket::OnReceive(nErrorCode);
}