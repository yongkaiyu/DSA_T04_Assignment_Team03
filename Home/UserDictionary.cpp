#include "UserDictionary.h"
#include "User.h"
#include "Member.h"
using namespace std;

// constructor
UserDictionary::UserDictionary() {
    size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

// destructor
UserDictionary::~UserDictionary() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp->user;   // delete User object
            delete temp; // delete node
        }
        table[i] = nullptr;
    }
}

/*
 Converts a userID string into a table index
 pre: key must not be empty
 post: returns a valid index that is in the table range
 Params: key - userID
 Returns: index between 0 and TABLE_SIZE - 1
 */
int UserDictionary::hashFunction(string& key) {
    //unsigned long hash = 5381;
    //for (char c : key) {
    //    hash = ((hash << 5) + hash) + c; // hash * 33 + c
    //}
    //return hash % TABLE_SIZE;

    int number = 0; //e.g of key = M014

    //extracting the numeric part of the id
    for (int i = 1; i < key.length(); i++) {
        number = number * 10 + (key[i] - '0');
    }

    return number % TABLE_SIZE;
}

/*
 Checks if a userID exists in the dictionary by searching for a userID
 Pre: userID must not be empty
 Post: none
 Params: userID - ID to search for
 Return: true if userID exists, false otherwise
 */
bool UserDictionary::contains(string& userID) {
    int index = hashFunction(userID);
    // traversing the linked list at the index
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == userID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/*
 Adds a new User into the dictionary by inserting a user using userID as the key
 Pre: user must not be nullptr
 Post: user is added if userID does not already exist
 Params: user - pointer to User object
 Return: true if user is successfully added, false otherwise
 */
bool UserDictionary::addUser(User* user) {
    if (user == nullptr) return false;

    string key = user->getUserID();
    int index = hashFunction(key);
    //checking for duplicate userid
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return false; // duplicate
        }
        current = current->next;
    }

    //insert new node at the top of the linked list
    Node* newNode = new Node;
    newNode->key = key;
    newNode->user = user;
    newNode->next = table[index];
    table[index] = newNode;

    size++;
    return true;
}

/*
Retrieves a User object by userID by returning the User pointer associated with the ID
Pre: userID must not be empty
Post: none
Params: userID - ID to search for
Return: pointer to User if found, nullptr otherwise
*/
User* UserDictionary::getUser(string& userID) {
    int index = hashFunction(userID);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == userID) {
            return current->user;
        }
        current = current->next;
    }
    return nullptr;
}
