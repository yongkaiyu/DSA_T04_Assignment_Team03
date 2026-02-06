#pragma once
#include "User.h"

using namespace std;

/*
	Team 3
	Team Member who did this feature:
	Daniel S10258472D

	Feature Highlight:
	Creating a new member and adding it to the system, generating a userid to go along with it
	Member is then stored in the user dictionary
*/

class UserDictionary;

class Admin : public User {
private:
	static int memberCount;
	string generateAutoID(); // generates unique member id like M001, M002, etc
	string adminID;
public:
	Admin(string UserID, string userName); // constructor for admin

	/*
	to create a new member and add it to the dictionary
	pre: member name must not be empty
	post: adds member to the dictionary and outmemberid is set to newid
	input params: reference to UserDictionary for id, member name, outmemberid?
	return value: returns ok if successfully added
	*/
	bool addMember(UserDictionary& user, string memberName, string& outMemberID);
};
