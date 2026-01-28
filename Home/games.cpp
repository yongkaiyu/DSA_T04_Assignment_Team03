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