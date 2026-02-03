#include "Booking.h"

Booking::Booking() {

	size = 0;
	nextBookingNumber = 1;
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

int Booking::hash(KeyType key)
{
	if (key.empty())
	{
		return -1;
	}

	unsigned long h = 5381;

	for (char c : key)
	{
		h = ((h << 5) + h) + c; // h * 33 + c
	}

	return int(h % MAX_SIZE);

}

string Booking::generateAutoID()
{
	return "B" + to_string(nextBookingNumber++);
}

bool Booking::borrowGame(string& userID, string& gameID)
{
	BookingData newItem;

	newItem.bookingID = generateAutoID();
	newItem.userID = userID;
	newItem.gameID = gameID;
	newItem.bookingIsReturned = false;
	KeyType newKey = newItem.bookingID;

	int index = hash(newKey);
	if (index < 0) return false; // safety

	if (items[index] == nullptr)
	{
		Node* newNode = new Node;
		newNode->key = newKey;
		newNode->item = newItem;
		newNode->next = nullptr;
		items[index] = newNode;
	}
	else
	{
		cout << "Collision at " << index << " - " << newKey << " and " << items[index]->key;
		cout << endl;
		Node* current = items[index];
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

bool Booking::returnGame(string& bookingID)
{
	int index = hash(bookingID);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			if (current->item.bookingIsReturned) {
				return false;
			}

			current->item.bookingIsReturned = true;
			return true;
		}
		current = current->next;
	}
	return false;
}

string Booking::getGameIDByBookingID(string& bookingID)
{
	int index = hash(bookingID);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			return current->item.gameID;
		}
		current = current->next;
	}
	return "";
}

bool Booking::bookingExists(string& bookingID)
{
	int index = hash(bookingID);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			return true;
		}
		current = current->next;
	}
	return false;
}

bool Booking::isBookingReturned(string& bookingID)
{
	int index = hash(bookingID);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			return current->item.bookingIsReturned;
		}
		current = current->next;
	}
	return false;
}

bool Booking::isUserBookingOwner(string& bookingID, string& userID)
{
	int index = hash(bookingID);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == bookingID)
		{
			return current->item.userID == userID;
		}
		current = current->next;
	}
	return false;
}

int Booking::countAllBookings() const
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

int Booking::collectAllBookings(BookingData arr[], int max)
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

static int extractGameNumber(const std::string& gameID)
{
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

static bool comesBeforeByGameID(const BookingData& a,
	const BookingData& b)
{
	int ga = extractGameNumber(a.gameID);
	int gb = extractGameNumber(b.gameID);

	// invalid IDs go last
	if (ga == -1 && gb != -1) return false;
	if (ga != -1 && gb == -1) return true;

	if (ga != gb)
		return ga < gb;

	// tie-breaker: bookingID (chronological)
	return a.bookingID < b.bookingID;
}

static void mergeByGameID(BookingData arr[],
	BookingData temp[],
	int left, int mid, int right)
{
	int i = left;      // left half pointer
	int j = mid + 1;   // right half pointer
	int k = left;      // temp array pointer

	while (i <= mid && j <= right)
	{
		if (comesBeforeByGameID(arr[i], arr[j]))
			temp[k++] = arr[i++];
		else
			temp[k++] = arr[j++];
	}

	// copy remaining left half
	while (i <= mid)
		temp[k++] = arr[i++];

	// copy remaining right half
	while (j <= right)
		temp[k++] = arr[j++];

	// copy back to original array
	for (int idx = left; idx <= right; idx++)
		arr[idx] = temp[idx];
}

static void mergeSortByGameID(BookingData arr[],
	BookingData temp[],
	int left, int right)
{
	if (left >= right)
		return;

	int mid = left + (right - left) / 2;

	mergeSortByGameID(arr, temp, left, mid);
	mergeSortByGameID(arr, temp, mid + 1, right);
	mergeByGameID(arr, temp, left, mid, right);
}

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

	for (int i = 0; i < count; i++)
	{
		std::cout << "GameID: " << arr[i].gameID
			<< " | BookingID: " << arr[i].bookingID
			<< " | UserID: " << arr[i].userID
			<< " | Returned: "
			<< (arr[i].bookingIsReturned ? "Yes" : "No")
			<< "\n";
	}

	delete[] temp;
	delete[] arr;
}


int Booking::countBookingsByUserID(const string& userID)
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

int Booking::collectBookingsByUserID(const string& userID, BookingData arr[], int max)
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

static int extractBookingNumber(const std::string& bookingID)
{
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

void insertionSortByBookingID(BookingData arr[], int n)
{
	for (int i = 1; i < n; i++)
	{
		BookingData key = arr[i];
		int keyNum = extractBookingNumber(key.bookingID);

		int j = i - 1;
		while (j >= 0 &&
			extractBookingNumber(arr[j].bookingID) < keyNum)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

void Booking::displaySortedByBookingID(const string& userID)
{

	int n = countBookingsByUserID(userID);

	if(n <= 0)
	{
		cout << "No bookings found.\n";
		return;
	}
	BookingData* arr = new BookingData[n];

	int count = collectBookingsByUserID(userID, arr, n);

	if (count == 0)
	{
		cout << "No bookings found.\n";
		return;
	}

	insertionSortByBookingID(arr, count);

	int returned = 0;

	for(int i = 0; i < count; i++)
	{
		if (arr[i].bookingIsReturned) {
			returned++;
		}
	}

	cout << "\n=== Your Booking Summary ===\n";
	for (int i = 0; i < count; i++)
	{
		cout << "Booking ID: " << arr[i].bookingID
			<< " | Game ID: " << arr[i].gameID
			<< " | Status: "
			<< (arr[i].bookingIsReturned ? "Returned" : "Active")
			<< endl;
	}

	cout << "---------------------------\n";
	cout << "Total bookings: " << count << endl;
	cout << "Returned bookings: " << returned << endl;
	cout << "Active bookings: " << count - returned << endl;

	delete[] arr;
}



/* BookingData Booking::get(KeyType key)
{
	int index = hash(key);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == key)
		{
			return current->item;
		}
		current = current->next;
	}

	return "";
} */
bool Booking::isEmpty()
{
	return size == 0;
}

int Booking::getLength()
{
	return size;
}

void Booking::printAdminSummary()
{
	int total = 0;
	int returned = 0;

	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (items[i] != nullptr)
		{
			Node* current = items[i];
			while (current != nullptr)
			{
				total++;
				cout << "Item: " << current->item.bookingID << "\n---\n";
				if (current->item.bookingIsReturned) {
					returned++;
				}
				current = current->next;
			}
		}
	}

	cout << "Total bookings: " << total << "\n";
	cout << "Returned bookings: " << returned << "\n";
	cout << "Active bookings: " << total - returned << "\n";
}

void Booking::printMemberSummary(string userID)
{
	int borrowed = 0;
	int returned = 0;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (items[i] != nullptr)
		{
			Node* current = items[i];
			while (current != nullptr)
			{
				if (current->item.userID == userID) {
					borrowed++;
					cout << "Item: " << current->item.bookingID << "\n---\n";
					if (current->item.bookingIsReturned) {
						returned++;
					}
				}
				current = current->next;
			}
		}
	}
	cout << "Total bookings for user " << userID << ": " << borrowed << "\n";
	cout << "Returned bookings: " << returned << "\n";
	cout << "Active bookings: " << borrowed - returned << "\n";
}