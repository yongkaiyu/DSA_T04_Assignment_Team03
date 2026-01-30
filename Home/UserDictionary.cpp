#include "UserDictionary.h"
#include "User.h"
#include "Member.h"
using namespace std;

UserDictionary::UserDictionary() {
    size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

UserDictionary::~UserDictionary() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp->user;   // delete the User object
            delete temp;
        }
        table[i] = nullptr;
    }
}

int UserDictionary::hashFunction(string& key) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % TABLE_SIZE;
}

bool UserDictionary::contains(string& userID) {
    int index = hashFunction(userID);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == userID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool UserDictionary::addUser(User* user) {
    if (user == nullptr) return false;

    std::string key = user->getUserID();
    int index = hashFunction(key);

    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return false; // duplicate
        }
        current = current->next;
    }

    Node* newNode = new Node;
    newNode->key = key;
    newNode->user = user;
    newNode->next = table[index];
    table[index] = newNode;

    size++;
    return true;
}

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
