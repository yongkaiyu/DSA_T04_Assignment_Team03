#include "User.h"
using namespace std;

User::User() {
	userID = "";
	userName = "";
	userRole = Role::Member;
}

User::User(string userID, string userName, Role userRole) 
{
	this->userID = userID;
	this->userName = userName;
	this->userRole = userRole;
}

User::~User() {

}

string User::getUserID() {
	return userID;
}

string User::getUserName() {
	return userName;
}

User::Role User::getRole() {
	return userRole;
}