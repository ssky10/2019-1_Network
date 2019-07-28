#include "pch.h"
#include "CClientSocket.h"
#include "CListenSocket.h"
#include "MainFrm.h"
#include "MFCApplication5Doc.h"
#include "MFCApplication5View.h"
#include "Item.h"
#include "CFreeDraw.h"
#include "CLine.h"
#include "CEllipse.h"
#include "CRectangle.h"
#include "CText.h"
#include "CImageItem.h"

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
	char socketBuffer[1025];
	TCHAR type;
	Receive(&type, sizeof(TCHAR)); //read the bigmap size
	if (type == '0') {
		int dimension, iLen;
		Receive(&dimension, sizeof(int)); //read the bigmap size

		char* bitmapBuffer = new char[dimension];
		int readSize = dimension;
		char* pBuffer = bitmapBuffer;
		while (readSize > 0)
		{
			int sizeToRead = readSize > sizeof(socketBuffer) ? sizeof(socketBuffer) : readSize;
			iLen = Receive(socketBuffer, sizeToRead, NULL);
			memcpy(pBuffer, socketBuffer, iLen);
			pBuffer += iLen;
			readSize -= iLen;
		}

		IStream* strm = SHCreateMemStream((BYTE*)pBuffer, dimension * sizeof(char));
		if (strm)
		{
			CImage img;
			HRESULT hResult = img.Load(strm);
			//InitPath();
			img.Save(_T("C:\Temp\NetPaint\Client\1111.jpg"), Gdiplus::ImageFormatJPEG);
			strm->Release();
		}
		else {
			AfxMessageBox(_T("이미지를 가져오는데 실패했습니다."));
		}
	}
	else if (type == '1') {
		DWORD dwReadLen;
		IOCtl(FIONREAD, &dwReadLen); //패킷 길이를 먼저 파악하고
		dwReadLen = (dwReadLen * sizeof(char)) / sizeof(TCHAR);
		TCHAR* szBuffer = new TCHAR[dwReadLen];
		//int nReadLen = 0;
		::ZeroMemory(szBuffer, sizeof(TCHAR) * dwReadLen);

		if (Receive(szBuffer, sizeof(TCHAR) * dwReadLen) > 0) { //전달된 데이터가 있을경우
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();

			CString str(szBuffer);
			//pDoc->list.push_back(new CText(CPoint(10, 10), str, 10, 1, 0));
			switch (str[0] - '0')
			{
			case Item::LINE:
				pDoc->addItem(new CLine(str));
				break;
			case Item::ELLIPSE:
				pDoc->addItem(new CEllipse(str));
				break;
			case Item::RECTANGLE:
				pDoc->addItem(new CRectangle(str));
				break;
			case Item::FREEDRAW:
				pDoc->addItem(new CFreeDraw(str));
				break;
			case Item::TEXT:
				pDoc->addItem(new CText(str));
				break;
			case Item::IMAGE:
				pDoc->addItem(new CImageItem(szBuffer));
				break;
			default:
				break;
			}
			pDoc->m_ListenSocket.SendAllMessage(pDoc->list[pDoc->list.size()-1]->DataToChar());
			pFrame->PostMessage(UM_UPDATEITEM, 0, 0);
		}
	}
	else if (type == '2') {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCApplication5Doc* pDoc = (CMFCApplication5Doc*)pFrame->GetActiveDocument();

		Receive(&type, sizeof(TCHAR)); //read the bigmap size

		for (int i = 0; i < pDoc->list.size(); i++) {
			if (pDoc->list[i]->ID == type - '0') {
				pDoc->m_ListenSocket.SendDeleteItem(pDoc->list[i]->ID);
				delete pDoc->list[i];
				pDoc->list.erase(pDoc->list.begin() + i);
				pFrame->PostMessage(UM_UPDATEITEM, 0, 0);
			}
		}
	}

	CSocket::OnReceive(nErrorCode);
}