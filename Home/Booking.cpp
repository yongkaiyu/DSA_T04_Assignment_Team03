#include "Booking.h"

Booking::Booking() {

	size = 0; // initialize size to 0
	nextBookingNumber = 1; // start booking numbers from 1
	for (int i = 0; i < MAX_SIZE; i++)
	{
		items[i] = nullptr;
	}
}
Booking::~Booking()
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		Node* current = items[i];

		while (current != nullptr)
		{
			Node* temp = current;
			current = current->next;
			delete temp;
		}

		items[i] = nullptr;
	}

	size = 0;

}

int Booking::hash(KeyType key) // djb2 hash function by Dan Bernstein
{
	if (key.empty())
	{
		return -1;
	}
	// 5381 is used as it is non-zero, odd number, ensuring early characters don't collapse to similar hashes. 0 would dominate the hash and make it ineffective.
	// Empirically, 5381 + x33 has low collision rates for string hashing.
	// Dan Bernstein empirically tested this as stable across varying string lengths, effective for ASCII strings and consistent on different platforms.
	// 5381 is just more well known, trusted and documented in hashing literature.
	unsigned long h = 5381;

	// hash each character in the key
	// shift left by 5 bits and add the current character
	// c is the ASCII value of the character (eg. 'A' = 65)
	for (char c : key)
	{
		h = ((h << 5) + h) + c; // ((h * (2 to the power of 5)) * h) + c -> (h * 33) + c
	}

	return int(h % MAX_SIZE); // ensure index is within table size

}

string Booking::generateAutoID() // generates booking IDs like B1, B2, B3, ...
{
	return "B" + to_string(nextBookingNumber++);
}

bool Booking::borrowGame(string& userID, string& gameID, string& bookingID) // add a new booking
{
	// Initialize a new BookingData item
	BookingData newItem;

	// Fill in the details
	newItem.bookingID = generateAutoID();
	newItem.userID = userID;
	newItem.gameID = gameID;
	newItem.borrowTime = time(nullptr);
	newItem.returnTime = 0;
	newItem.bookingIsReturned = false;
	KeyType newKey = newItem.bookingID;

	bookingID = newItem.bookingID; // return the generated booking ID

	// Hash the key to get the index
	int index = hash(newKey);

	if (index < 0) return false; // ensure that the index is valid

	if (items[index] == nullptr) // no collision, directly insert
	{
		Node* newNode = new Node;
		newNode->key = newKey;
		newNode->item = newItem;
		newNode->next = nullptr;
		items[index] = newNode;
	}
	else // collision occurred, use separate chaining
	{
		cout << endl;
		Node* current = items[index];
		// Linked list is unsorted, use sequential search
		if (current->key == newKey)
		{
			return false;
		}
		while (current->next != nullptr)
		{
			current = current->next;
			if (current->key == newKey)
			{
				return false;
			}
		}
		Node* newNode = new Node;
		newNode->key = newKey;
		newNode->item = newItem;
		newNode->next = nullptr;
		current->next = newNode;
	}

	size++;
	return true;
}

bool Booking::returnGame(string& bookingID) // update booking to mark as returned
{
	// find the booking by bookingID
	int index = hash(bookingID);

	Node* current = items[index];
	// Linked list is unsorted, use sequential search
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			if (current->item.bookingIsReturned) { // already returned
				return false;
			}

			current->item.bookingIsReturned = true;
			return true;
		}
		current = current->next;
	}
	return false; // bookingID not found
}

string Booking::getGameIDByBookingID(string& bookingID) // retrieve gameID by bookingID
{
	// find the booking by bookingID
	int index = hash(bookingID);

	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID) // found
		{
			return current->item.gameID;
		}
		current = current->next;
	}
	return ""; // bookingID not found
}

bool Booking::bookingExists(string& bookingID) // check if booking exists
{
	// find the booking by bookingID
	int index = hash(bookingID);

	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID) // found
		{
			return true;
		}
		current = current->next;
	}
	return false; // bookingID not found
}

bool Booking::isBookingReturned(string& bookingID) // check if booking is returned
{
	// find the booking by bookingID
	int index = hash(bookingID);

	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			return current->item.bookingIsReturned; // return true if returned
		}
		current = current->next;
	}
	return false; // bookingID not found
}

bool Booking::isUserBookingOwner(string& bookingID, string& userID) // check if user is owner of booking
{
	// find the booking by bookingID
	int index = hash(bookingID);

	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			return current->item.userID == userID; // return true if user is owner
		}
		current = current->next;
	}
	return false;
}

bool Booking::isBookingOverdue(const BookingData& booking, long currentTime) // check if booking is overdue
{
	const long BORROW_DURATION = 30L * 24 * 60 * 60; // one month
	if (booking.bookingIsReturned || booking.borrowTime <= 0)
	{
		return false; // already returned
	}
	long dueTime = booking.borrowTime + BORROW_DURATION;
	return currentTime > dueTime; // overdue if current time is past due time
}

int Booking::countAllBookings() const // count all bookings
{
	int count = 0;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		Node* cur = items[i];
		while (cur != nullptr)
		{
			count++;
			cur = cur->next;
		}
	}
	return count;
}

int Booking::collectAllBookings(BookingData arr[], int max) // collect all bookings and store in arr
{
	int count = 0;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		Node* cur = items[i];
		while (cur != nullptr && count < max)
		{
			arr[count++] = cur->item;
			cur = cur->next;
		}
	}
	return count;
}

static int extractGameNumber(const std::string& gameID) // extract numeric part from gameID like G1, G2, ...
{
	// verify format
	if (gameID.size() < 2) return -1;
	if (gameID[0] != 'G') return -1;

	int num = 0;
	for (size_t i = 1; i < gameID.size(); i++)
	{
		char c = gameID[i];
		if (c < '0' || c > '9') return -1;
		num = num * 10 + (c - '0');
	}
	return num;
}

static bool comesBeforeByGameID(const BookingData& a, const BookingData& b) // comparison function for sorting by gameID
{
	int ga = extractGameNumber(a.gameID);
	int gb = extractGameNumber(b.gameID);

	// invalid IDs go last
	if (ga == -1 && gb != -1) return false; // a is invalid, b is valid
	if (ga != -1 && gb == -1) return true; // a is valid, b is invalid

	if (ga != gb) // compare by game number
		return ga < gb;

	// tie-breaker: bookingID (chronological)
	return a.bookingID < b.bookingID;
}

static void mergeByGameID(BookingData arr[], BookingData temp[], int left, int mid, int right) // merge two halves for merge sort
{
	int i = left;      // left half pointer
	int j = mid + 1;   // right half pointer
	int k = left;      // temp array pointer

	// merge the two halves into temp[]
	while (i <= mid && j <= right)
	{
		if (comesBeforeByGameID(arr[i], arr[j])) // compare by gameID
			temp[k++] = arr[i++]; // take from left half
		else
			temp[k++] = arr[j++]; // take from right half
	}

	// copy remaining left half
	while (i <= mid)
		temp[k++] = arr[i++]; // take from left half

	// copy remaining right half
	while (j <= right)
		temp[k++] = arr[j++]; // take from right half

	// copy back to original array
	for (int idx = left; idx <= right; idx++)
		arr[idx] = temp[idx];
}

// recursive merge sort by gameID
// Why merge sort?
// Merge sort is efficient with O(n log n) time complexity, stable (preserves order of equal elements), and works well for linked structures.
// It is suitable for sorting large datasets, which is ideal for admin functions that may need to handle many bookings.
static void mergeSortByGameID(BookingData arr[], BookingData temp[], int left, int right)
{
	if (left >= right) // base case
		return;

	int mid = left + (right - left) / 2; // avoid overflow

	// Recursively sort left and right halves
	mergeSortByGameID(arr, temp, left, mid);
	mergeSortByGameID(arr, temp, mid + 1, right);
	mergeByGameID(arr, temp, left, mid, right);
}

// Admin function to display all bookings sorted by gameID
// Why by gameID?
// GameID groups bookings of the same game together, making it easier for admins to see all bookings for a specific game in one place.
// This is useful for managing inventory, tracking popular games, and handling returns efficiently.
void Booking::displayAllSortedByGameID()
{
	int n = countAllBookings();
	if (n <= 0) {
		std::cout << "No bookings.\n";
		return;
	}

	BookingData* arr = new BookingData[n];
	BookingData* temp = new BookingData[n];

	int count = collectAllBookings(arr, n);
	if (count > 1)
		mergeSortByGameID(arr, temp, 0, count - 1);

	long now = time(nullptr);

	// overall totals
	int totalAll = 0, returnedAll = 0, activeAll = 0, overdueAll = 0;

	int i = 0;
	while (i < count)
	{
		string currentGame = arr[i].gameID;

		int total = 0, returned = 0, active = 0, overdue = 0;

		// process one group (same gameID)
		while (i < count && arr[i].gameID == currentGame)
		{
			total++;
			totalAll++;

			if (arr[i].bookingIsReturned) {
				returned++; returnedAll++;
			}
			else {
				active++; activeAll++;
				if (isBookingOverdue(arr[i], now)) { overdue++; overdueAll++; }
			}

			i++;
		}

		cout << currentGame
			<< " -> Total: " << total
			<< " | Active: " << active
			<< " | Returned: " << returned
			<< " | Overdue: " << overdue
			<< "\n";
	}

	cout << "-------------------------\n";
	cout << "Overall -> Total: " << totalAll
		<< " | Active: " << activeAll
		<< " | Returned: " << returnedAll
		<< " | Overdue: " << overdueAll
		<< "\n";

	delete[] temp;
	delete[] arr;
}

int Booking::countBookingsByUserID(const string& userID) // count bookings by specific userID
{
	int count = 0;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		Node* cur = items[i];
		while (cur != nullptr)
		{
			if (cur->item.userID == userID)
				count++;
			cur = cur->next;
		}
	}
	return count;
}

int Booking::collectBookingsByUserID(const string& userID, BookingData arr[], int max) // collect bookings by specific userID
{
	int count = 0;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		Node* cur = items[i];
		while (cur != nullptr && count < max)
		{
			if (cur->item.userID == userID) { 
				arr[count++] = cur->item;
			}
			cur = cur->next;
		}
	}
	return count;
}

static int extractBookingNumber(const std::string& bookingID) // extract numeric part from bookingID like B1, B2, ...
{
	// verify format
	if (bookingID.size() < 2) return -1;
	if (bookingID[0] != 'B') return -1;

	int num = 0;
	for (size_t i = 1; i < bookingID.size(); i++)
	{
		char c = bookingID[i];
		if (c < '0' || c > '9') return -1;
		num = num * 10 + (c - '0');
	}
	return num;
}

void insertionSortByBookingID(BookingData arr[], int n) // insertion sort by bookingID in descending order
{
	for (int i = 1; i < n; i++)
	{
		BookingData key = arr[i];
		int keyNum = extractBookingNumber(key.bookingID);

		int j = i - 1;
		while (j >= 0 &&
			extractBookingNumber(arr[j].bookingID) < keyNum)
		{
			arr[j + 1] = arr[j]; // shift right
			j--;
		}
		arr[j + 1] = key; // insert key
	}
}

// Member function to display bookings by specific userID sorted by bookingID
// Why by bookingID?
// BookingID reflects the chronological order of bookings, allowing users to see their most recent bookings first.
// This is useful for members to track their borrowing history and manage returns effectively.
void Booking::displaySortedByUserID(const string& userID)
{
	int n = countBookingsByUserID(userID);

	if (n <= 0)
	{
		cout << "No bookings found.\n";
		return;
	}
	BookingData* arr = new BookingData[n];

	int count = collectBookingsByUserID(userID, arr, n);

	if (count == 0)
	{
		cout << "No bookings found.\n";
		delete[] arr;
		return;
	}

	insertionSortByBookingID(arr, count);

	long now = time(nullptr);   // get current time once
	int returned = 0;
	int overdue = 0;

	cout << "\n=== Your Booking Summary ===\n";
	for (int i = 0; i < count; i++)
	{
		string status;

		if (arr[i].bookingIsReturned)
		{
			status = "Returned";
			returned++;
		}
		else if (isBookingOverdue(arr[i], now))
		{
			status = "Overdue";
			overdue++;
		}
		else
		{
			status = "Active";
		}

		cout << "Booking ID: " << arr[i].bookingID
			<< " | Game ID: " << arr[i].gameID
			<< " | Status: " << status
			<< endl;
	}

	cout << "---------------------------\n";
	cout << "Total bookings: " << count << endl;
	cout << "Returned bookings: " << returned << endl;
	cout << "Overdue bookings: " << overdue << endl;
	cout << "Active bookings: " << (count - returned - overdue) << endl;

	delete[] arr;
}