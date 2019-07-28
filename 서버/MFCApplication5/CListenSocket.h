#pragma once
#include <afxsock.h>
class CListenSocket :
	public CAsyncSocket
{
public:
	CListenSocket();
	~CListenSocket();

	CPtrList m_ptrClientSocketList;
	CPtrList waitClientSocketList;
	void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* pClient);
	void SendAllMessage(TCHAR* pszMessage);
	void SendImage(char* pszMessage, int len);
	void SendDeleteItem(int itemID);
};

