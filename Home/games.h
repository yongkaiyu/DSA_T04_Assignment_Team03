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

    //Review* reviewsHead = nullptr; // Pointer to the list of reviews for this gam
};

/*struct Review {
    std::string memberID;
    std::string comment;
    float rating;
    Review* next; // To form a linked list of reviews
};*/

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

    Game* searchGame(std::string id) const;

    int searchByPrefixPaged(const std::string& prefix, Game results[], int maxResults, int startIndex) const;

    void displayGameMatches(const Game games[], int count) const;

    void displayAll(); // part i 

    int removeGame(std::string name); //removes one copy of the game from the dictionary or else delete 

    // Updates the rating of a specific game
    float rateGame(std::string id, float rating);

	int getAvailableCopiesForGameByID(std::string name);

	bool borrowGameUpdateTotalCopies(std::string gameID);

	bool returnGameUpdateTotalCopies(std::string gameID);

	bool gameExists(std::string gameID);


	//view games menu logic
    void displayGameDetails(std::string id);

    // Filter and sort logic
    //void displayFilteredGames(int players, std::string sortBy);

    // Adds a review to a specific game
    //bool addReview(std::string gameID, std::string memberID, std::string comment, float rating);

    // Displays all reviews for a specific game
    //void displayReviews(std::string gameID);
};