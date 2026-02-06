#pragma once
#include "User.h"

using namespace std;

class UserDictionary;

class Admin : public User {
private:
	static int memberCount;
	string generateAutoID();
	string adminID;
public:
	Admin(string UserID, string userName);

	bool addMember(UserDictionary& user, string memberName, string& outMemberID);
};
