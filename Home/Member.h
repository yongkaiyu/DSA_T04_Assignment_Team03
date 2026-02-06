#pragma once
#include "User.h"

/*
    Team 3
    Team Member who did this feature:
    Daniel S10258472D

    Feature Highlight:
    Inherits from User class and differs from admins
*/

class Member :public User {
private:

public:
	Member(string userID, string userName); // constructor, creates a new member object
};
