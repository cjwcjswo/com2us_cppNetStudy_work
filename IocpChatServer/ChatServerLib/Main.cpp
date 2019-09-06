
#include "Main.h"


namespace ChatServerLib 
{
	Main::Main() 
	{
	}

	Main::~Main()
	{
	}

	int Main::Init(ChatServerConfig serverConfig) //Config�� �ܺο��� �޵��� �� �Լ��� ��
	{
		m_pIOCPServer = std::make_unique<NetLib::IOCPServerNet>();
		
		auto netConfig = serverConfig.GetNetConfig();

		NetLib::E_NET_RESULT ServerStartResult = m_pIOCPServer->StartServer(netConfig);
		if (ServerStartResult != NetLib::E_NET_RESULT::Success) {
			printf("ServerStartError! ErrorCode: %d\n", (int)ServerStartResult);
			return -1;
		}		

		m_pPacketManager = std::make_unique<PacketManager> ();
		m_pUserManager = std::make_unique<UserManager> ();
		m_pRoomManager = std::make_unique<RoomManager> ();

		m_pPacketManager->Init(m_pIOCPServer.get(), m_pUserManager.get(), m_pRoomManager.get());
		
		m_pUserManager->Init();

		m_pRoomManager->SendPacketFunc = [&](INT32 connectionIndex, void* pSendPacket, INT16 packetSize)
		{
			m_pIOCPServer->SendPacket(connectionIndex, pSendPacket, packetSize);
		};
		m_pRoomManager->Init();

		max_packet_size = m_pIOCPServer->GetMaxPacketSize();
		max_connection_count = m_pIOCPServer->GetMaxConnectionCount();
		post_message_thread_cnt = m_pIOCPServer->GetPostMessagesThreadsCount();

				
		//���������� �ʱ�ȭ�ϰ� UserManager
		
		return 0;
	}
		
	void Main::Run() 
	{
		m_IsRun = true;

		int MaxPacketSize = Common::MAX_PACKET_SIZE + 1;
		char* pBuf = new char[MaxPacketSize];
		ZeroMemory(pBuf, sizeof(char) * MaxPacketSize);
		INT32 waitTimeMillisec = 1;

		while (m_IsRun)
		{
			INT8 operationType = 0;
			INT32 connectionIndex = 0;
			INT16 copySize = 0;

			//���� �Լ� �����ϱ�(���� �����Ҵ� ��� �Ǿ)

			//WorkThread�� �Լ����� �ҷ��ͼ� ó���Ѵ�.

			if (m_pIOCPServer->ProcessNetworkMessage(operationType, connectionIndex, pBuf, copySize, waitTimeMillisec))
			{
				switch (operationType)
				{
				case NetLib::OP_CONNECTION:
					printf("On Connect %d\n",connectionIndex);
					//	m_pPacketManager->ConnectClient(connectionIndex);
					break;
				case NetLib::OP_CLOSE:
					m_pPacketManager->ClearConnectionInfo(connectionIndex);
					break;
				case NetLib::OP_RECV_PACKET:
					m_pPacketManager->ProcessRecvPacket(connectionIndex, pBuf, copySize);
					break;
				}
			}
		}

	}

	void Main::Stop()
	{
		m_IsRun = false;

		m_pIOCPServer->EndServer();

	}

	
	
}