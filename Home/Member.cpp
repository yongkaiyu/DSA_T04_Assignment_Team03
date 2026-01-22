#include "Member.h"

Member::Member(string userID, string userName) :User(userID, userName, Role::Member) {
	totalBorrowed = 0;
	totalReturned = 0;
}

void Member::incrementBorrowed() {
	totalBorrowed++;
}

void Member::incrementReturned() {
	totalReturned++;
}

int Member::getTotalBorrowed() {
	return totalBorrowed;
}

int Member::getTotalReturned() {
	return totalReturned;
}