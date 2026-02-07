#pragma once
#include <string>
#include <iostream>
using namespace std;
#include "UserDictionary.h"

/*
    Team 3
    Team Member who did this feature:
    Daniel S10258472D

    Feature Highlight:
    Advanced feature: Records a play of a game (including players and the winner)
    Data Structures: Linked list of play records; linked list of players per record
*/

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

    /*
    to generate the unique playid
    pre: none
    post: generates a unique playID
    input params: none
    return value: string containing the generated ID
    */
    string generatePlayID();

    /*
    checks if userid already exists in linked list, to prevent duplicates
    pre: player list, userID
    post: bool value of whether the player already exists (true for exists, false otherwise)
    input params: player list, userID
    return value: returns true or false on whether the player is in the list
    */
    // checks if a player already exists in the player list
    bool playerExistsInList(PlayerNode* list, string userID);

public:
    PlayHistory();
    ~PlayHistory();

    /*
    Adding a play record using game id, game name, the winner's id. a string to store all the participants
    pre: a player record should exist first
    post: new play added
    input params: gameID, gameName, winnerID (memberID of winner), playerIDs[] (array of member ids involved), playercount, number of players in playerIDs[]
    Return value: returns true is play record is successfully stored, false if input is invalid or winner not in player list
    */
    bool addPlay(string gameID, string gameName, string winnerID, string playerIDs[], int playerCount);

    /*
    Print all recorded plays
    pre: none
    post: recorded plays are displayed
    input params: users- Reference to UserDictionary for ID
    Return value: none
    */
    void printAll(UserDictionary& users);

    /*
    Print all recorded plays after sorting them by game name
    pre: none
    post: recorded plays are displayed after sorting
    input params: users- Reference to UserDictionary for ID
    Return value: none
    */
    void printSortedByGame(UserDictionary& users);
};
