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

    PlayRecord* first;
    int playCount;

    // generates the unique playid
    string generatePlayID();

    // checks if a player already exists in the player list
    bool playerExistsInList(PlayerNode* list, string userID);

public:
    PlayHistory();
    ~PlayHistory();

    // Adding a play record using game id, game name, the winner's id. a string to store all the participants
    bool addPlay(string gameID, string gameName, string winnerID, string playerIDs[], int playerCount);

    // Print all recorded plays
    void printAll(UserDictionary& users);
};
