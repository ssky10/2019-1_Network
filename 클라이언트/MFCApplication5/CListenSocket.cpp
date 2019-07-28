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

//����-Ŭ���̾�Ʈ ������ ����ϴ� �Լ�
void CListenSocket::OnAccept(int nErrorCode) {
	CClientSocket* pClient = new CClientSocket;
	CString str;

	if (Accept(*pClient)) { //Ŭ���̾�Ʈ ���ӿ�û�� ���� ���
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient);
		str.Format(_T("Client (%d)"), (int)m_ptrClientSocketList.Find(pClient));
		pDoc->clientList.AddTail(str); //Ŭ���̾�Ʈ�� �����Ҷ� ���� ����Ʈ�� �߰�
	}
	else {
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

//Ŭ���̾�Ʈ�� ������ ����
void CListenSocket::CloseClientSocket(CSocket* pClient) {
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);

	if (pos != NULL) {
		if (pClient != NULL) {
			//Ŭ���̾�Ʈ �������� �� ����
			pClient->ShutDown();
			pClient->Close();
		}
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
		CString str1;
		UINT indx = 0, posNum;
		//������ ����Ǵ� Ŭ���̾�Ʈ ã��
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
			//send �Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
			//�� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
			int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
			if (checkLenOfData != lstrlen(pszMessage) * 2) {
				AfxMessageBox(_T("�Ϻ� �����Ͱ� ���������� ���۵��� ���߽��ϴ�."));
			}
		}
	}
}