#include "User.h"
#include "Admin.h"
#include "Member.h"
#include "UserDictionary.h"

Admin::Admin(string userID, string userName)
    : User(userID, userName, Role::Admin) {
}

bool Admin::addMember(UserDictionary& users,
    string memberID,
    string memberName) {
    if (memberID.empty() || memberName.empty()) {
        return false;
    }

    if (users.contains(memberID)) {
        return false; // duplicate ID
    }

    Member* newMember = new Member(memberID, memberName);
    users.addUser(newMember);

    return true;
}