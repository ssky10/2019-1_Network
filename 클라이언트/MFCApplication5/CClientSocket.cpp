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
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { //���޵� �����Ͱ� �������
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
		strTmp.Format(_T("[%s:%d]:%s"), strIPAddress, uPortNumber, strBuffer);
		//pMain->m_List.AddString(strTmp); //�޽��� ����Ʈ�� �Է¹��� �޽��� ����
		//pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);
		
		CListenSocket * pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->SendAllMessage(strBuffer); //�ٸ� Ŭ���̾�Ʈ�鿡�� �޽��� ����
	}

	CSocket::OnReceive(nErrorCode);
}