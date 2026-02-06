#pragma once
#include <string>
#include <iostream>

using namespace std;

/*
	Team 3
	Team Member who did this feature:
	Daniel S10258472D

	Feature Highlight:
	Base class for the system user, stores userid, username and user role, parent class for Admin and Member
*/

class User {
public:
	enum class Role { // defines the type of user in the system like admin and member
		Admin,
		Member
	};

protected:
	string userID;
	string userName;
	Role userRole;

public:
	User(); // constructor

	virtual ~User(); // destructor

	User(string userID, string userName, Role userRole); // constructor with user id, username and user role

	string getUserID(); // gets user id
	string getUserName(); // gets user name
	Role getRole(); //gets user's role
};


