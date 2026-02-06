#include "Member.h"

/*
    Team 3
    Team Member who did this feature:
    Daniel S10258472D

    Feature Highlight:
    Inherits from User class and differs from admins
*/

// calls the base user constructor and to initialise user id, user name and user role (set to member)
Member::Member(string userID, string userName) :User(userID, userName, Role::Member) {
	
}
