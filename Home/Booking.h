#pragma once
// Booking.h - Specification of Booking ADT

// Done by Yong Kai Yu - S10258484E

#include<string>
#include<iostream>
using namespace std;

const int MAX_SIZE = 101;
typedef string KeyType; // bookingID

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
	BookingData item;	// data item
	Node* next;	// pointer pointing to next item with same search key
};

/*
	Done By:
	Name: Yong Kai Yu
	Student ID: S10258484E
	Class: T04
	Group: Team3
	Features: Hash Table with Separate Chaining, with BookingData as data item.
*/
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

	// hash function to map keys to table indices
	// pre : none
	// post: none
	// return the hash index for the given key
	int hash(KeyType key);

	// generate a new unique booking ID
	// pre : none
	// post: none
	// return a new unique booking ID
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

	// get the game ID associated with a booking ID
	// pre : bookingID must exist in the Booking
	// post: none
	// return the game ID associated with the booking ID
	string getGameIDByBookingID(string& bookingID);

	// check if a booking with the specified booking ID exists
	// pre : none
	// post: none
	// return true if the booking exists; otherwise returns false
	bool bookingExists(string& bookingID);

	// check if a booking has been returned
	// pre : bookingID must exist in the Booking
	// post: none
	// return true if the booking has been returned; otherwise returns false
	bool isBookingReturned(string& bookingID);

	// check if the user is the owner of the booking
	// pre : bookingID must exist in the Booking
	// post: none
	// return true if the user is the owner of the booking; otherwise returns false
	bool isUserBookingOwner(string& bookingID, string& userID);

	// Algorithm functions

	// count all bookings in the Booking
	// pre : none
	// post: none
	// return the number of bookings in the Booking
	int countAllBookings() const;

	// collect all bookings in the Booking
	// pre : arr has enough space to hold all bookings
	// post: arr contains all bookings in the Booking
	// return the number of bookings collected
	int collectAllBookings(BookingData arr[], int max);

	// display all bookings sorted by game ID (Admin Function)
	// pre : none
	// post: bookings are displayed sorted by game ID
	// return: none
	void displayAllSortedByGameID();

	// count bookings by a specific user ID
	// pre : none
	// post: none
	// return the number of bookings made by the specified user ID
	int countBookingsByUserID(const string& userID);
	
	// collect bookings by a specific user ID
	// pre : arr has enough space to hold bookings by the specified user ID
	// post: arr contains bookings made by the specified user ID
	// return the number of bookings collected
	int collectBookingsByUserID(const string& userID, BookingData arr[], int max);

	// display bookings by a specific user ID sorted by booking ID (Member Function)
	// pre : none
	// post: bookings are displayed sorted by booking ID
	// return: none
	void displaySortedByUserID(const string& userID);

};

