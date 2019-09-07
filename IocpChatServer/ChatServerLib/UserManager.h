#pragma once
#include <unordered_map>

#include "User.h"



namespace ChatServerLib
{
	class UserManager
	{

	public:
		UserManager() = default;
		~UserManager() = default;

		void Init()
		{
			UserObjPool = std::vector<User*>(MAX_USER_CNT);

			for (int i = 0; i < MAX_USER_CNT; i++)
			{
				UserObjPool[i] = new User();
				UserObjPool[i]->Init(i);
			}
		}

		int GetCurrentUserCnt() { return current_user_cnt; } 

		int GetMaxUserCnt() { return MAX_USER_CNT; }
		
		void IncreaseUserCnt() { current_user_cnt++; }
		
		void DecreaseUserCnt() 
		{
			if (current_user_cnt > 0) 
			{
				current_user_cnt--;
			}
		}

		int AddUser(char* userID, int conn_idx)
		{
			//TODO ����� ���� �ߺ� �����ϱ�

			int user_idx = conn_idx;

			UserObjPool[user_idx]->SetLogin(conn_idx, userID);
			UserDictionary.insert(std::pair< char*, int>(userID, conn_idx));

			return 0;
		}
		
		int FindUserByID(char* userID)
		{
			if (auto res = UserDictionary.find(userID); res != UserDictionary.end())
			{
				return (*res).second;
			}
			
			return -1;
		}
		
		void DeleteUserInfo(User* deleteUser)
		{
			UserDictionary.erase(deleteUser->GetUserId());
			deleteUser->Clear();
		}

		User* GetUserByConnIdx(INT32 conn_idx)
		{
			return UserObjPool[conn_idx];
		}


	private:
		const int MAX_USER_CNT = 1024; //TODO ����� �� ���� �ϵ��ڵ��� �ƴ� �ܺο��� ���� �޵��� �Ѵ�
		int current_user_cnt = 0; //TODO ����� �ڵ� �꿡 �°� �̸� �ٲٱ�

		std::vector<User*> UserObjPool; //vector��
		std::unordered_map<std::string, int> UserDictionary;
	};
}