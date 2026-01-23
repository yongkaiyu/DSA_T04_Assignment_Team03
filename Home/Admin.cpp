#include "User.h"
#include "Admin.h"
#include "Member.h"
#include "UserDictionary.h"

int Admin::memberCount = 0;

string Admin::generateAutoID() {
    memberCount++;
    char id[5];
    id[0] = 'M';
    id[1] = char('0' + (memberCount / 100) % 10);
    id[2] = char('0' + (memberCount / 10) % 10);
    id[3] = char('0' + (memberCount / 10));
    id[4] = char('\0');

    return string(id);
}

Admin::Admin(string userID, string userName)
    : User(userID, userName, Role::Admin) {
}

bool Admin::addMember(UserDictionary& users, string memberName) {
    if (memberName.empty()) return false;

    std::string newID = generateAutoID();

    // to avoid collision
    while (users.contains(newID)) {
        newID = generateAutoID();
    }

    Member* m = new Member(newID, memberName);
    return users.addUser(m);
}