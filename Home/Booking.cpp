#include "Booking.h"

Booking::Booking() {

	size = 0;
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

bool Booking::borrowGame(KeyType newKey, ItemType newItem)
{
	int index = hash(newKey);

	if (items[index] == nullptr)
	{
		Node* newNode = new Node;
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

bool Booking::returnGame(KeyType key)
{
	int index = hash(key);
	Node* current = items[index];
	while (current != nullptr)
	{
		if (current->key == key)
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

ItemType Booking::get(KeyType key)
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
}
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
				cout << "Item: " << current->item << "\n---\n";
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
				if (current->item.bookingUserID == userID) {
					borrowed++;
					cout << "Item: " << current->item << "\n---\n";
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