#include "User.h"
#include "Admin.h"
#include "Member.h"
#include "UserDictionary.h"

/*
    Team 3
    Team Member who did this feature:
    Daniel S10258472D

    Feature Highlight:
    Creating a new member and adding it to the system, generating a userid to go along with it
    Data Structures: Dictionary
*/

int Admin::memberCount = 0;

/*
to generate the unique memberid
pre: none
post: generates a unique memberID
input params: none
return value: string containing the generated ID
*/
string Admin::generateAutoID() {
    memberCount++;
    char id[5];
    id[0] = 'M';
    id[1] = char('0' + (memberCount / 100) % 10);
    id[2] = char('0' + (memberCount / 10) % 10);
    id[3] = char('0' + (memberCount % 10));
    id[4] = char('\0');

    return string(id);
}

Admin::Admin(string userID, string userName)
    : User(userID, userName, Role::Admin) {
}

/*
to create a new member and add it to the dictionary
pre: member has to be created first for it to be added to the dictionary
post: adds member to the dictionary
input params: reference to UserDictionary for id, member name, outmemberid?
return value: returns ok if successfully added
*/
bool Admin::addMember(UserDictionary& users, string memberName, string& outMemberID) {
    if (memberName.empty()) return false;

    string newID = generateAutoID();

    // to avoid collision
    while (users.contains(newID)) {
        newID = generateAutoID();
    }

    Member* m = new Member(newID, memberName);
    bool ok = users.addUser(m);

    if (ok) {
        outMemberID = newID;
    }
    return ok;
}