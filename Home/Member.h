#pragma once
#include "User.h"

class Member :User {
private:
	int totalBorrowed;
	int totalReturned;

public:
	Member(string userID, string userName);

	// placeholders
	void incrementBorrowed();
	void incrementReturned();

	int getTotalBorrowed();
	int getTotalReturned();

};
