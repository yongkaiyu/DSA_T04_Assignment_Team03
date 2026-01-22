#pragma once
#include "User.h"

using namespace std;

class UserDictionary;

class Admin : User {
public:
	Admin(string UserID, string userName);

	bool addMember(UserDictionary& user, string UserID, string userName);
};
