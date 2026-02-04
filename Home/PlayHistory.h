#pragma once
#include <string>
#include <iostream>
using namespace std;
#include "UserDictionary.h"

class PlayHistory {
private:
    struct PlayerNode {
        string userID;
        PlayerNode* next;
    };

    struct PlayRecord {
        string playID;
        string gameName;
        string gameID;
        string winnerID;
        PlayerNode* players;
        PlayRecord* next;
    };

    PlayRecord* head;
    int playCount;

    string generatePlayID();
    bool playerExistsInList(PlayerNode* list, string userID);

public:
    PlayHistory();
    ~PlayHistory();

    // Adding a play record
    bool addPlay(string gameID, string gameName, string winnerID, string playerIDs[], int playerCount);

    // Print all plays
    void printAll(UserDictionary& users);

    // Print plays for a specific member
    void printByUserID(string userID, UserDictionary& users);

    // Print plays for a specific game
    void printByGameID(string gameID, UserDictionary& users);
};
