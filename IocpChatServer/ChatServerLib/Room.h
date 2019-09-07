#pragma once

#include <functional>
#include "UserManager.h"

namespace ChatServerLib
{
	class Room 
	{
	public:
		Room() = default;
		~Room() = default;

		void Init(const INT32 roomNum);

		UINT16 GetMaxUserCount() { return MAX_USER_COUNT; }
		
		UINT16 GetCurrentUserCount() { return m_CurrentUserCount; }
		
		INT32 GetRoomNumber() { return m_RoomNum; }

		UINT16 EnterUser(User* pUser);
		
		UINT16 LeaveUser(User* pUser);
						
		void NotifyChat(INT32 connIndex, const char* UserID, const char* Msg);
		
		void SendToAllUser(const UINT16 dataSize, void* pData, const INT32 passUserindex, bool exceptMe);

		
		std::function<void(INT32, void*, INT16)> SendPacketFunc;

	private:
		INT32 m_RoomNum = -1;

		std::vector<User*> m_UserList;
		//ToDo �뿡�� ����ڸ� �̸��ְ�, Enter�� �Ǹ�
		//1. ���� �� �ִ� ��������(UserList�� �ִ� ������)
		

		UINT16 MAX_USER_COUNT = 2; //1:1�� Ư���� ����ؼ� 2������ ����
		UINT16 m_CurrentUserCount = 0;
	};
}