#include "User.h"
using namespace std;

/*
	Team 3
	Team Member who did this feature:
	Daniel S10258472D

	Feature Highlight:
	Base class for the system user, stores userid, username and user role, parent class for Admin and Member
*/

// constructor, sets it to member role by default
User::User() {
	userID = "";
	userName = "";
	userRole = Role::Member;
}

// constructor with parameters user id, user name and user role
User::User(string userID, string userName, Role userRole) 
{
	// assigning values to the member variables
	this->userID = userID;
	this->userName = userName;
	this->userRole = userRole;
}

// virtual destructor
User::~User() {

}

// returns user id
string User::getUserID() {
	return userID;
}

// returns user name
string User::getUserName() {
	return userName;
}

// returns user's role
User::Role User::getRole() {
	return userRole;
}