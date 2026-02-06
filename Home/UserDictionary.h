#pragma once
#include <string>
#include "User.h"
using namespace std;

/*
	Team 3
	Team Member who did this feature:
	Daniel S10258472D

	Feature Highlight:
	Implements a dictionary (hash table) to store the users
	Users stored and retrieved using the userid as key
*/

class UserDictionary {
private:
	struct Node {
		string key; // user id (as hash key)
		User* user; // pointer to user object (admin or member)
		Node* next; // pointer to next node in the chain
	};

	static const int TABLE_SIZE = 101; // size of hash table, in prime number to prevent conflicts
	Node* table[TABLE_SIZE]; // array of pointers representing the hash table buckets
	int size; // number of users currently stored

	/*
	Converts a userID string into a table index
	pre: key must not be empty
	post: returns a valid index that is in the table range
	Params: key - userID
	Returns: index between 0 and TABLE_SIZE - 1
	*/
	int hashFunction(string& key);

public:
	UserDictionary(); // constructor
	~UserDictionary(); // destructor

	/*
	Adds a new User into the dictionary by inserting a user using userID as the key
	Pre: user must not be nullptr
	Post: user is added if userID does not already exist
	Params: user - pointer to User object
	Return: true if user is successfully added, false otherwise
	*/
	bool addUser(User* user);

	/*
	Checks if a userID exists in the dictionary by searching for a userID
	Pre: userID must not be empty
	Post: none
	Params: userID - ID to search for
	Return: true if userID exists, false otherwise
	*/
	bool contains(string& userID);

	/*
	Retrieves a User object by userID by returning the User pointer associated with the ID
	Pre: userID must not be empty
	Post: none
	Params: userID - ID to search for
	Return: pointer to User if found, nullptr otherwise
	*/
	User* getUser(string& userID);
};
