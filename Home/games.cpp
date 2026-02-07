#include "Games.h"
#include <iostream>
#include <iomanip>

GameDictionary::GameDictionary() : gameCount(0) {
    for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
}

int GameDictionary::hashFunction(std::string key) {
    unsigned int hash = 0;
    for (char c : key) hash = (hash * 31) + c;
    return hash % TABLE_SIZE;
}

Game* GameDictionary::searchByName(std::string name) {
    int index = hashFunction(name);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == name) return &(current->data);
        current = current->next;
    }
    return nullptr;
}


//Search by ID


Game* GameDictionary::searchGame(std::string id) {
    // We cannot use hashFunction(id) because the table is indexed by Name.
    // We must loop through the entire table to find the specific ID.
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];

        while (current != nullptr) {
            // Check if this node's gameID matches "G001", "G002", etc.
            if (current->data.gameID == id) {
                return &(current->data);
            }
            current = current->next;
        }
    }
    return nullptr; // If we've checked everything and found nothing
}


static char lowerChar(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
    return c;
}

static bool startsWithIgnoreCase(const std::string& text, const std::string& prefix)
{
    if (prefix.size() > text.size()) return false;
    for (size_t i = 0; i < prefix.size(); i++)
        if (lowerChar(text[i]) != lowerChar(prefix[i])) return false;
    return true;
}

int GameDictionary::searchByPrefixPaged(const std::string& prefix, Game results[], int max, int startIndex) const
{
    if (max <= 0 || startIndex < 0) return 0;
    if (prefix.empty()) return 0;

    int skipped = 0;
    int count = 0;

    for (int i = 0; i < TABLE_SIZE && count < max; i++)
    {
        Node* cur = table[i];
        while (cur != nullptr && count < max)
        {
            if (startsWithIgnoreCase(cur->data.gameName, prefix))
            {
                if (skipped < startIndex) skipped++;
                else results[count++] = cur->data;
            }
            cur = cur->next;
        }
    }
    return count;
}

void GameDictionary::displayGameMatches(const Game games[], int count) const
{
    if (count <= 0)
    {
        std::cout << "No matching games found.\n";
        return;
    }

    std::cout << "\nMatching games:\n";
    for (int i = 0; i < count; i++)
    {
        std::cout << games[i].gameID << " | "
            << games[i].gameName
            << " | Available: "
            << games[i].gameAvailableCopies << "/"
            << games[i].gameTotalCopies
            << std::endl;
    }
}

#include <sstream> // Required for ostringstream

void GameDictionary::addOrUpdateGame(Game g) {
    // 1. Check if the game already exists in the dictionary first
    Game* existing = searchByName(g.gameName);

    if (existing) {
        // Game found -> just update the copies, don't increase gameCount
        existing->gameTotalCopies++;
        existing->gameAvailableCopies++;
        return;
    }

    // 2. New game detected -> Increment the counter
    gameCount++;

    // 3. Format the ID with leading zeros (e.g., G001)
    std::ostringstream oss;
    oss << "G" << std::setfill('0') << std::setw(3) << gameCount;
    g.gameID = oss.str();

    // 4. Initialize the new game settings
    g.gameTotalCopies = 1;
    g.gameAvailableCopies = 1;

    // 5. Store in the Hash Table
    int index = hashFunction(g.gameName);
    Node* newNode = new Node{ g.gameName, g, table[index] };
    table[index] = newNode;
}


// Returns the number of copies left, or -1 if the game was not found
int GameDictionary::removeGame(std::string name) {
    int index = hashFunction(name);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == name) {
            // Found the game
            if (current->data.gameTotalCopies > 1) {
                // More than one copy exists: just decrement counts
                current->data.gameTotalCopies--;
                current->data.gameAvailableCopies--;
                return current->data.gameTotalCopies; // Return remaining count
            }
            else {
                // Only one copy left: remove the entire node from the list
                if (prev == nullptr) {
                    table[index] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                return 0; // Return 0 to indicate the game is completely gone
            }
        }
        prev = current;
        current = current->next;
    }
    return -1; // Return -1 if the game was not found
}

float GameDictionary::rateGame(std::string id, float rating) {
    // Basic range check
    if (rating < 1.0f || rating > 10.0f) return -1.0f;

    Game* found = searchGame(id);

    if (found != nullptr) {
        if (found->gameAverageRating == 0.0f) {
            found->gameAverageRating = rating;
        }
        else {
            // Running average logic
            found->gameAverageRating = (found->gameAverageRating + rating) / 2.0f;
        }
        return found->gameAverageRating; // Return the new value
    }

    return -1.0f; // Indicate failure (ID not found)
}

int GameDictionary::getAvailableCopiesForGameByID(std::string gameID) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            if (current->data.gameID == gameID) {
                return current->data.gameAvailableCopies;
            }
            current = current->next;
        }
    }
    return -1; // Game not found
}

bool GameDictionary::borrowGameUpdateTotalCopies(std::string gameID) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            if (current->data.gameID == gameID) {
                if (current->data.gameAvailableCopies > 0) {
                    current->data.gameAvailableCopies--;
                    return true; // Successfully borrowed
                }
                else {
                    return false; // No available copies
                }
            }
            current = current->next;
        }
    }
    return false; // Game not found
}

bool GameDictionary::returnGameUpdateTotalCopies(std::string gameID) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            if (current->data.gameID == gameID) {
                if (current->data.gameAvailableCopies < current->data.gameTotalCopies) {
                    current->data.gameAvailableCopies++;
                    return true; // Successfully returned
                }
                else {
                    return false; // All copies are already available
                }
            }
            current = current->next;
        }
    }
    return false; // Game not found
}

bool GameDictionary::gameExists(std::string gameID) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            if (current->data.gameID == gameID) {
                return true; // Game found
            }
            current = current->next;
        }
    }
    return false; // Game not found
}

void GameDictionary::displayAll() { //DISPLAYS EVERYTHING
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            std::cout << "ID: " << current->data.gameID
                      << ", Name: " << current->data.gameName
				      << ", Min Players: " << current->data.gameMinPlayer
				      << ", Max Players: " << current->data.gameMaxPlayer
				      << ", Min Play Time: " << current->data.gameMinPlayTime
				      << ", Max Play Time: " << current->data.gameMaxPlayTime
				      << ", Year Published: " << current->data.gameYearPublished
				      << ", Average Rating: " << std::fixed << std::setprecision(2) << current->data.gameAverageRating
                      << ", Total Copies: " << current->data.gameTotalCopies
                      << ", Available Copies: " << current->data.gameAvailableCopies
                      << std::endl;
            current = current->next;
        }
	}
}

void GameDictionary::displayGameDetails(std::string id) { //Displays a particular game details
    Game* found = searchGame(id); 
    if (found) {
        std::cout << "\n--- Showing Details for Game ID: " << id << " ---\n";
        std::cout << "Name: " << found->gameName
            << "\nPlayers: " << found->gameMinPlayer << "-" << found->gameMaxPlayer
            << "\nPlay Time: " << found->gameMinPlayTime << "-" << found->gameMaxPlayTime << " mins"
            << "\nYear Published: " << found->gameYearPublished
            << "\nAverage Rating: " << std::fixed << std::setprecision(2) << found->gameAverageRating
            << "\nTotal Copies: " << found->gameTotalCopies
            << "\nAvailable Copies: " << found->gameAvailableCopies << std::endl;
    }
    else {
        std::cout << "Game with ID " << id << " not found." << std::endl;
    }
}


GameDictionary::~GameDictionary() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
}

/*bool GameDictionary::addReview(std::string gameID, std::string memberID, std::string comment, float rating) {
    Game* found = searchGame(gameID);
    if (found) {
        // 1. Create a new Review node
        Review* newReview = new Review;
        newReview->memberID = memberID;
        newReview->comment = comment;
        newReview->rating = rating;

        // 2. Insert at the head of the reviews list (simplest way)
        newReview->next = found->reviewsHead;
        found->reviewsHead = newReview;

        // 3. Automatically update the game's average rating
        rateGameByID(gameID, rating);

        return true;
    }
    return false;
}

void GameDictionary::displayReviews(std::string gameID) {
    Game* found = searchGame(gameID);
    if (found) {
        std::cout << "\n--- Reviews for " << found->gameName << " ---\n";
        Review* current = found->reviewsHead;

        if (!current) {
            std::cout << "No reviews yet for this game.\n";
            return;
        }

        while (current) {
            std::cout << "Member: " << current->memberID << "\n";
            std::cout << "Rating: " << current->rating << "/10\n";
            std::cout << "Review: " << current->comment << "\n";
            std::cout << "---------------------------\n";
            current = current->next;
        }
    }
    else {
        std::cout << "Game ID not found.\n";
    }
}*/