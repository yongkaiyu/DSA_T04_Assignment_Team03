#pragma once

#include<string>
#include<iostream>
using namespace std;

const int ACTIVE_MAX_SIZE = 101;
typedef string KeyType; // userID|gameID
typedef string ItemType; // bookingID

struct ActiveNode {
	KeyType key; // userID|gameID
	ItemType item; // bookingID
    ActiveNode* next;
};

/*
    Done By:
    Name: Yong Kai Yu
    Student ID: S10258484E
    Class: T04
    Group: Team3
	Features: Hash Table with Separate Chaining, where key is userID|gameID and item is bookingID.
	Details: This ActiveBookingIndex class manages active bookings by mapping a combination of userID and gameID to a bookingID.
             It helps check if a user already borrowed a game and
			 ensures one user cannot borrow the same game multiple times without returning it first.
			 Using this index, userID | gameID, it prevents duplicates as for example
			 U1 cannot borrow G3 twice concurrently = there can only be one U1 | G3 entry at any time.
			 Also, usually it takes O(n) time to search as it searches through all bookings,
			 but with this index, it can be done in O(1) average time using a hash.
*/

// (Advanced Feature) - Data structures that help improve performance and/or storage efficiency

class ActiveBookingIndex {
private:
    ActiveNode* items[ACTIVE_MAX_SIZE];
    int size;

public:
    ActiveBookingIndex();
    ~ActiveBookingIndex();

	// hash function to map keys to table indices
	// pre : none
	// post: none
	// return the hash index for the given key
    int hash(KeyType key) const;

	// create a composite key from userID and gameID
	// pre : none
	// post: none
	// return the composite key
    string makeKey(const string& userID, const string& gameID) const;

	// add a new active booking entry
	// pre : none
	// post: new active booking entry is added
	// return true if added successfully; false if already exists
    bool addActive(const string& userID, const string& gameID, const string& bookingID);

	// remove an active booking entry
	// pre : none
	// post: active booking entry is removed
	// return true if removed successfully; false if not found
    bool removeActive(const string& userID, const string& gameID);

	// check if an active booking entry exists
	// pre : none
	// post: none
	// return true if exists; false otherwise
    bool hasActive(const string& userID, const string& gameID) const;

	// retrieve bookingID for an active booking entry
	// pre : none
	// post: none
	// return bookingID if found; empty string otherwise
    string getBookingID(const string& userID, const string& gameID) const;
};

