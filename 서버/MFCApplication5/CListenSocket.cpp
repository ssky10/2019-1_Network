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

//����-Ŭ���̾�Ʈ ������ ����ϴ� �Լ�
void CListenSocket::OnAccept(int nErrorCode) {
	CClientSocket* pClient = new CClientSocket;
	CString str;

	if (Accept(*pClient)) { //Ŭ���̾�Ʈ ���ӿ�û�� ���� ���
		CString strIPAddress = _T("");
		UINT uPortNumber = 0;

		pClient->GetPeerName(strIPAddress, uPortNumber);
		str.Format(_T("[%s:%d]�� ������ �㰡�Ͻðڽ��ϱ�?"), (LPCWSTR)strIPAddress, uPortNumber);
		if (AfxMessageBox(str,MB_YESNO) == IDYES) {
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();
			pClient->SetListenSocket(this);
			m_ptrClientSocketList.AddTail(pClient);
			str.Format(_T("Client (%d)"), uPortNumber);
			pDoc->clientList.AddTail(str); //Ŭ���̾�Ʈ�� �����Ҷ� ���� ����Ʈ�� �߰�
			pFrame->PostMessage(UM_CLIENTUPDATE, 0, 0);

			//������� �׷��� ���� ��ü ����
			for each (Item* var in pDoc->list)
			{
				SendAllMessage(var->DataToChar()); //�ٸ� Ŭ���̾�Ʈ�鿡�� �޽��� ����
				Sleep(10);
			}
		}
		else {
			if (pClient != NULL) {
				//Ŭ���̾�Ʈ �������� �� ����
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
		POSITION cpos = pDoc->clientList.GetHeadPosition();
		posNum = (int)m_ptrClientSocketList.Find(pClient);
		str1.Format(_T("%d"), posNum);
		while (cpos != NULL) {
			CString string = pDoc->clientList.GetAt(cpos);
			if (string.Find(str1) != -1) {
				AfxMessageBox(str1 + _T("Client ���� ����"));
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
			//send �Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
			//�� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
			int checkLenOfData = pClient->Send(pszMessage, (lstrlen(pszMessage) + 1) * 2);
			if (checkLenOfData != (lstrlen(pszMessage)+1)*2) {
				AfxMessageBox(_T("�Ϻ� �����Ͱ� ���������� ���۵��� ���߽��ϴ�."));
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
			//send �Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
			//�� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
			int checkLenOfData = pClient->Send(pszMessage, len);
			if (checkLenOfData != len) {
				AfxMessageBox(_T("�Ϻ� �����Ͱ� ���������� ���۵��� ���߽��ϴ�."));
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
			//send �Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
			//�� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
			int checkLenOfData = pClient->Send((TCHAR*)(LPCTSTR)(msg), (msg.GetLength() + 1) * 2);
			if (checkLenOfData != (msg.GetLength() + 1) * 2) {
				AfxMessageBox(_T("�Ϻ� �����Ͱ� ���������� ���۵��� ���߽��ϴ�."));
			}
		}
	}
}
