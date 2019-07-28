#include "pch.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "MainFrm.h"
#include "MFCApplication5Doc.h"
#include "MFCApplication5View.h"


CListenSocket::CListenSocket()
{
}


CListenSocket::~CListenSocket()
{
}

//서버-클라이언트 연결을 담당하는 함수
void CListenSocket::OnAccept(int nErrorCode) {
	CClientSocket* pClient = new CClientSocket;
	CString str;

	if (Accept(*pClient)) { //클라이언트 접속요청이 왔을 경우
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient);
		str.Format(_T("Client (%d)"), (int)m_ptrClientSocketList.Find(pClient));
		pDoc->clientList.AddTail(str); //클라이언트가 접속할때 마다 리스트에 추가
	}
	else {
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

//클라이언트의 연결을 종료
void CListenSocket::CloseClientSocket(CSocket* pClient) {
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);

	if (pos != NULL) {
		if (pClient != NULL) {
			//클라이언트 연결중지 후 종료
			pClient->ShutDown();
			pClient->Close();
		}
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
		CString str1;
		UINT indx = 0, posNum;
		//접속이 종료되는 클라이언트 찾기
		posNum = (int)m_ptrClientSocketList.Find(pClient);
		str1.Format(_T("%d"), posNum);
		pDoc->clientList.RemoveAt(pDoc->clientList.Find(str1));
		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}

void CListenSocket::SendAllMessage(TCHAR * pszMessage) {
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			//send 함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
			//이 함수는 전송한 데이터의 길이를 반환한다.
			int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
			if (checkLenOfData != lstrlen(pszMessage) * 2) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다."));
			}
		}
	}
}