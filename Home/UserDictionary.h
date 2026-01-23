#pragma once
#include <string>
#include "User.h"

class UserDictionary {
private:
	struct Node {
		string key;
		User* user;
		Node* next;
	};

	static const int TABLE_SIZE = 101;
	Node* table[TABLE_SIZE];
	int size;

	int hashFunction(string& key);

public:
	UserDictionary();
	~UserDictionary();

	bool addUser(User* user);
	bool contains(string& userID);
	User* getUser(string& userID);
};
