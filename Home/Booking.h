#pragma once
// Booking.h - Specification of Booking ADT

#include<string>
#include<iostream>
using namespace std;

const int MAX_SIZE = 101;
typedef string KeyType; // bookingID
typedef BookingData ItemType; // booking data

struct BookingData
{
	string bookingID;
	string userID;
	string gameID;
	bool bookingIsReturned;
};

// Why are bookingBorrowDate and bookingReturnDate long?
// Backend is kept simple, searching and sorting are easier with long integers representing dates in the format YYYYMMDD.
// DateTime requires to define on own, use of more memory, more code for comparison, and in the context of the assignment, it is overkill.
// String dates would require harder comparisons, and manual parsing for comparison and manipulation, there is slower sorting and searching which is inefficient.

struct Node
{
	KeyType	 key;   // search key
	ItemType item;	// data item
	Node* next;	// pointer pointing to next item with same search key
};

class Booking
{
private:

	Node* items[MAX_SIZE];
	int size;			// number of items in the Booking
	int nextBookingNumber; // to generate unique booking IDs

public:

	// constructor
	Booking();

	// destructor
	~Booking();

	int hash(KeyType key);

	string generateAutoID();

	// add a new item with the specified key to the Booking
	// pre : none
	// post: new item is added to the Booking
	// size of Booking is increased by 1
	bool borrowGame(string& userID, string& gameID);

	// update an item with the specified key in the Booking
	// pre : key must exist in the Booking
	// post: item is updated in the Booking
	// size of Booking is the same
	bool returnGame(string& bookingID);

	// get an item with the specified key in the Booking (retrieve)
	// pre : key must exist in Booking
	// post: none
	// return the item with the specified key from Booking
	ItemType get(KeyType key);

	// check if the Booking is empty
	// pre : none
	// post: none
	// return true if the Booking is empty; otherwise returns false
	bool isEmpty();

	// check the size of the Booking
	// pre : none
	// post: none
	// return the number of items in the Booking
	int getLength();

	//------------------- Other useful functions -----------------

	// display the items in the Booking
	void printAdminSummary();

	void printMemberSummary(string userID);
};

