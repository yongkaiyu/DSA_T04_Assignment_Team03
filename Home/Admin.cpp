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
    Member is then stored in the user dictionary
*/

int Admin::memberCount = 0; // initialising static member count

/*
to generate the unique memberid
pre: none
post: generates a unique memberID
input params: none
return value: string containing the generated ID
*/
string Admin::generateAutoID() {
    // increment static count first
    memberCount++;
    char id[5];
    id[0] = 'M';
    id[1] = char('0' + (memberCount / 100) % 10);
    id[2] = char('0' + (memberCount / 10) % 10);
    id[3] = char('0' + (memberCount % 10));
    id[4] = char('\0'); // terminates null

    return string(id); // converts the char array into string and return
}

Admin::Admin(string userID, string userName)
    : User(userID, userName, Role::Admin) {
}

/*
to create a new member and add it to the dictionary
pre: member name must not be empty
post: adds member to the dictionary and outmemberid is set to newid
input params: reference to UserDictionary for id, member name, outmemberid?
return value: returns ok if successfully added
*/
bool Admin::addMember(UserDictionary& users, string memberName, string& outMemberID) {
    if (memberName.empty()) return false;

    string newID = generateAutoID(); // generates the id and store it as the new id

    // to avoid collision
    while (users.contains(newID)) {
        newID = generateAutoID();
    }

    Member* m = new Member(newID, memberName); // create the new member
    bool success = users.addUser(m); // attempting to insert member to dictionary

    // if insertion fails, cleans up memory to avoid leakage
    if (!success) {
        delete m;
        return false;
    }
    outMemberID = newID;
    return true;
}