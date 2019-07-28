#include "pch.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "MainFrm.h"
#include "MFCApplication5Doc.h"
#include "MFCApplication5View.h"
#include "CImageItem.h"


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
		CString strIPAddress = _T("");
		UINT uPortNumber = 0;

		pClient->GetPeerName(strIPAddress, uPortNumber);
		str.Format(_T("[%s:%d]의 접속을 허가하시겠습니까?"), (LPCWSTR)strIPAddress, uPortNumber);
		if (AfxMessageBox(str,MB_YESNO) == IDYES) {
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
			pClient->SetListenSocket(this);
			m_ptrClientSocketList.AddTail(pClient);
			str.Format(_T("Client (%d)"), uPortNumber);
			pDoc->clientList.AddTail(str); //클라이언트가 접속할때 마다 리스트에 추가
			pFrame->PostMessage(UM_CLIENTUPDATE, 0, 0);

			//현재까지 그려진 내용 전체 전송
			for each (Item* var in pDoc->list)
			{
				SendAllMessage(var->DataToChar()); //다른 클라이언트들에게 메시지 전달
				Sleep(10);
			}
		}
		else {
			if (pClient != NULL) {
				//클라이언트 연결중지 후 종료
				pClient->ShutDown();
				pClient->Close();
			}
		}
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
		POSITION cpos = pDoc->clientList.GetHeadPosition();
		posNum = (int)m_ptrClientSocketList.Find(pClient);
		str1.Format(_T("%d"), posNum);
		while (cpos != NULL) {
			CString string = pDoc->clientList.GetAt(cpos);
			if (string.Find(str1) != -1) {
				AfxMessageBox(str1 + _T("Client 접속 종료"));
				pDoc->clientList.RemoveAt(cpos);
				break;
			}
			pDoc->clientList.GetNext(cpos);
		}
		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
		pFrame->PostMessage(UM_CLIENTUPDATE, 0, 0);
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
			int checkLenOfData = pClient->Send(pszMessage, (lstrlen(pszMessage) + 1) * 2);
			if (checkLenOfData != (lstrlen(pszMessage)+1)*2) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다."));
			}
		}
	}
}

void CListenSocket::SendImage(char* pszMessage, int len) {
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			//send 함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
			//이 함수는 전송한 데이터의 길이를 반환한다.
			int checkLenOfData = pClient->Send(pszMessage, len);
			if (checkLenOfData != len) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다."));
			}
		}
	}
}

void CListenSocket::SendDeleteItem(int itemID) {
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	CString msg;

	msg.Format(_T("%d%d"),2,itemID);

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			//send 함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
			//이 함수는 전송한 데이터의 길이를 반환한다.
			int checkLenOfData = pClient->Send((TCHAR*)(LPCTSTR)(msg), (msg.GetLength() + 1) * 2);
			if (checkLenOfData != (msg.GetLength() + 1) * 2) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다."));
			}
		}
	}
}
