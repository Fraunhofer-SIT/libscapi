#pragma once
//socket.h

#ifndef __SOCKET_H__BY_SGCHOI 
#define __SOCKET_H__BY_SGCHOI 

#include "typedefs.h"
#include <transputation/transputation.h>
//using boost::asio::ip::tcp;

namespace semihonestot {

	class CSocket
	{
	public:
		CSocket() { 
			t = transputation::Transport::GetTransport("tcp");
		}
		~CSocket() { }
		//~CSocket(){ cout << "Closing Socket!" << endl; Close(); }

	public:
		BOOL Socket()
		{
			return t != NULL;
		}

		void Close()
		{
			delete t;
		}

		void AttachFrom(CSocket& s)
		{
			printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}

		void Detach()
		{
		}

	public:
		string GetIP()
		{
			return ip;
		}


		USHORT GetPort()
		{
			return port;
		}

		BOOL Bind(USHORT nPort = 0, string ip = "")
		{
			this->port = nPort;
			this->ip = ip;

			return TRUE;
		}

		BOOL Listen(int nQLen = 5)
		{
			t->SetupServer(ip.c_str(), port);
			return TRUE;
		}

		BOOL Accept(CSocket& sock)
		{
			t->Accept();

			return TRUE;
		}

		BOOL Connect(string ip, USHORT port, LONG lTOSMilisec = -1)
		{
			t->SetupClient(ip.c_str(), port);
			t->Connect();
			return TRUE;
		}

		int Receive(void* pBuf, int nLen)
		{
			return t->RecvRaw(nLen, (uint8_t *)pBuf);
		}

		int Send(const void* pBuf, int nLen, int nFlags=0)
		{
			return t->SendRaw(nLen, (uint8_t *)pBuf);
		}

	private:
		transputation::Transport *t;
		string ip = "";
		USHORT port = 0;
#ifdef Z_USE_BOOST_ASIO
		//acceptor acc;
#endif
	};

}
#endif //SOCKET_H__BY_SGCHOI

