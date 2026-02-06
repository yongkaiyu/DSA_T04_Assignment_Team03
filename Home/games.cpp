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
    int index = hashFunction(id);

    Node* current = table[index];
    while (current != nullptr) {
        // Compare the gameID in the data struct with the search ID
        if (current->data.gameID == id) {
            return &(current->data); // Return the address of the game object
        }
        current = current->next; // Move to the next node in the chain
    }
    return nullptr;
}


void GameDictionary::addOrUpdateGame(Game g) {
    Game* existing = searchByName(g.gameName);

    if (existing) {
        // If game exists, just increase the copy counts
        existing->gameTotalCopies++;
        existing->gameAvailableCopies++;
    }
    else {
        // New game: Generate an ID like G001, G002...
        gameCount++;
        g.gameID = "G" + std::to_string(gameCount);
        g.gameTotalCopies = 1;
        g.gameAvailableCopies = 1;

        int index = hashFunction(g.gameName);
        Node* newNode = new Node{ g.gameName, g, table[index] };
        table[index] = newNode;
    }
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

bool GameDictionary::addReview(std::string gameID, std::string memberID, std::string comment, float rating) {
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
}