#pragma once

#include <string>

struct Game {
    std::string gameID;
    std::string gameName;
    int gameMinPlayer;
    int gameMaxPlayer;
    int gameMinPlayTime;
    int gameMaxPlayTime;
    int gameYearPublished;
    float gameAverageRating;
    int gameTotalCopies;
    int gameAvailableCopies;
};

class GameDictionary {
private:
    struct Node {
        std::string key; // We will use gameName as the key for lookups
        Game data;
        Node* next;
    };

    static const int TABLE_SIZE = 101;
    Node* table[TABLE_SIZE];
    int hashFunction(std::string key);
    int gameCount; // Used to generate unique IDs (e.g., G001, G002)

public:
    GameDictionary();
    ~GameDictionary();

    // Logic: If name exists, increment copies. If not, create new ID and add.
    void addOrUpdateGame(Game g);
    Game* searchByName(std::string name);
    void displayAll();

    bool removeGame(std::string name);
};