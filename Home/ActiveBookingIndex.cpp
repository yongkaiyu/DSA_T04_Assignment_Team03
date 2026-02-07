#include "ActiveBookingIndex.h"

ActiveBookingIndex::ActiveBookingIndex() {

	size = 0;

	for (int i = 0; i < ACTIVE_MAX_SIZE; i++) {

		items[i] = nullptr;

	}
	
}

ActiveBookingIndex::~ActiveBookingIndex() {

	for (int i = 0; i < ACTIVE_MAX_SIZE; i++) {

		ActiveNode* current = items[i];

		while (current != nullptr) {

			ActiveNode* temp = current;
			current = current->next;
			delete temp;

		}

		items[i] = nullptr;
	}
	size = 0;
}

string ActiveBookingIndex::makeKey(const string& userID, const string& gameID) const{
	return userID + "|" + gameID;
}

int ActiveBookingIndex::hash(KeyType key) const{

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

	return int(h % ACTIVE_MAX_SIZE); // ensure index is within table size
}

bool ActiveBookingIndex::addActive(const string& userID, const string& gameID, const string& bookingID) { // add a new active booking entry

	if (userID.empty() || gameID.empty() || bookingID.empty()) return false;

	KeyType key = makeKey(userID, gameID);
	int index = hash(key);
	if (index < 0) return false;

	// Check if already exists (no duplicates)
	ActiveNode* cur = items[index];
	while (cur != nullptr)
	{
		if (cur->key == key)
			return false; // already active
		cur = cur->next;
	}

	// Insert at head (simple and safe)
	ActiveNode* node = new ActiveNode;
	node->key = key;
	node->item = bookingID;
	node->next = items[index];
	items[index] = node;

	size++;
	return true;
}

bool ActiveBookingIndex::removeActive(const string& userID, const string& gameID) { // remove an active booking entry

	if (userID.empty() || gameID.empty()) return false;

	KeyType key = makeKey(userID, gameID);
	int index = hash(key);
	if (index < 0) return false;

	ActiveNode* current = items[index];
	ActiveNode* prev = nullptr;

	// Find and remove the node with the matching key
	while (current != nullptr) {
		if (current->key == key) {
			if (prev == nullptr) {
				items[index] = current->next;
			}
			else {
				prev->next = current->next;
			}
			delete current;
			size--;
			return true;
		}
		prev = current;
		current = current->next;
	}
	return false; // not found
}

bool ActiveBookingIndex::hasActive(const string& userID, const string& gameID) const { // check if an active booking entry exists

	if (userID.empty() || gameID.empty()) return false;

	KeyType key = makeKey(userID, gameID);
	int index = hash(key);
	if (index < 0) return false;

	ActiveNode* current = items[index];
	while (current != nullptr) {
		if (current->key == key) {
			return true;
		}
		current = current->next;
	}
	return false;
}

bool ActiveBookingIndex::hasActiveUser(const string& userID) const { // check if any active booking entry exists for userID
	if (userID.empty()) return false;
	for (int i = 0; i < ACTIVE_MAX_SIZE; i++)
	{
		ActiveNode* cur = items[i];
		while (cur != nullptr)
		{
			// Extract userID from key
			size_t delimiterPos = cur->key.find('|');
			if (delimiterPos != string::npos)
			{
				string keyUserID = cur->key.substr(0, delimiterPos);
				if (keyUserID == userID)
					return true;
			}
			cur = cur->next;
		}
	}
	return false;
}

// Retrieve bookingID for an active (userID, gameID)
// Returns "" if not found
string ActiveBookingIndex::getBookingID(const string& userID, const string& gameID) const
{
	if (userID.empty() || gameID.empty()) return "";

	string key = makeKey(userID, gameID);
	int index = hash(key);
	if (index < 0) return "";

	ActiveNode* cur = items[index];
	while (cur != nullptr)
	{
		if (cur->key == key)
			return cur->item;
		cur = cur->next;
	}
	return "";
}