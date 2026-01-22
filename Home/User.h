#pragma once
#include <string>
#include <iostream>

using namespace std;

class User {
public:
	enum class Role {
		Admin,
		Member
	};

protected:
	string userID;
	string userName;
	Role userRole;

public:
	User();

	virtual ~User();

	User(string userID, string userName, Role userRole);

	string getUserID();
	string getUserName();
	Role getRole();
};


