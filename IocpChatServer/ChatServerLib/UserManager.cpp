#include <utility>

#include "UserManager.h"


namespace ChatServerLib
{
	//TODO ����� ũ�Ⱑ �����Ƿ� .cpp�� �Լ� ������ .h ���Ϸ� �̵��Ѵ�
	void UserManager::Init() {

		UserObjPool = std::vector<User*>(MAX_USER_CNT);

		for (int i = 0; i < MAX_USER_CNT; i++) {
			UserObjPool[i] = new User();
			UserObjPool[i]->Init(i);
		}


	}

	//TODO �ϴ� void������ �ϴٰ� ���� ErrorCode�� �ʿ��ϴٸ� �̸� ����ϴ� Enum�� �����
	//TODO ����� ���� �ߺ� �����ϱ�
	int UserManager::AddUser(char* userID, int conn_idx)
	{
		int user_idx = conn_idx;

		UserObjPool[user_idx]->SetLogin(conn_idx, userID);
		UserDictionary.insert(std::pair< char*, int>(userID, conn_idx));

		return 0;
	}

	//TODO ����� �ڵ� �ϰ��� �ʿ� 
	//    {
	//} �� �Ʒ�ó�� �ٲ۴�
	// {
	// }
	int UserManager::FindUserByID(char* userID) {

		std::unordered_map<std::string, int>::iterator res;
		res = UserDictionary.find(userID);
		if (res == UserDictionary.end()) {
			return -1;
		}
		else {
			return (*res).second;
		}
	}

	void UserManager::DeleteUserInfo(User* deleteUser) {
		UserDictionary.erase(deleteUser->GetUserId());
		deleteUser->Clear();

	}

	User* UserManager::GetUserByConnIdx(INT32 conn_idx)
	{
		return UserObjPool[conn_idx];
	}
}