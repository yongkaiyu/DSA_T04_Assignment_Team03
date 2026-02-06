#include "PlayHistory.h"
#include "UserDictionary.h"
using namespace std;

/*
    Team 3
    Team Member who did this feature:
    Daniel S10258472D

    Feature Highlight:
    Advanced feature: Records a play of a game (including players and the winner)
    Data Structures: Linked list of play records; linked list of players per record
*/

/*
Initialises PlayHistory
Creates an empty play history linked list
Input params: none
Return value: none
*/
PlayHistory::PlayHistory() {
    first = nullptr;
    playCount = 0; // initialises it to empty play history list
}

/*
Destroys PlayHistory
Frees all memory used by the play records linked list and the players linked list inside each record
Input params: none
Return value: none
*/
PlayHistory::~PlayHistory() {
    PlayRecord* current = first;
    while (current != nullptr) {
        // delete players list
        PlayerNode* p = current->players;
        while (p != nullptr) {
            PlayerNode* tempPlayer = p;
            p = p->next;
            delete tempPlayer;
        }

        // delete record node (play record) itself
        PlayRecord* tempRecord = current;
        current = current->next;
        delete tempRecord;
    }
    first = nullptr;
}

/*
to generate the unique playid
pre: none
post: generates a unique playID
input params: none
return value: string containing the generated ID
*/
string PlayHistory::generatePlayID() {
    playCount++;

    // "P" + 3 digits (P001...)
    char id[5];
    id[0] = 'P';
    id[1] = char('0' + (playCount / 100) % 10);
    id[2] = char('0' + (playCount / 10) % 10);
    id[3] = char('0' + (playCount % 10));
    id[4] = '\0';

    return string(id);
}

/*
checks if userid already exists in linked list, to prevent duplicates
pre: player list, userID
post: bool value of whether the player already exists (true for exists, false otherwise)
input params: player list, userID
return value: returns true or false on whether the player is in the list
*/
bool PlayHistory::playerExistsInList(PlayerNode* list, string userID) {
    PlayerNode* current = list;
    while (current != nullptr) {
        if (current->userID == userID) return true;
        current = current->next;
    }
    return false;
}

/*
Adding a play record using game id, game name, the winner's id. a string to store all the participants
pre: a player record should exist first
post: new play added
input params: gameID, gameName, winnerID (memberID of winner), playerIDs[] (array of member ids involved), playercount, number of players in playerIDs[]
Return value: returns true is play record is successfully stored, false if input is invalid or winner not in player list
*/
bool PlayHistory::addPlay(string gameID, string gameName, string winnerID, string playerIDs[], int playerCount) {
    if (gameID.empty() || winnerID.empty())
    {
        return false;
    }
    if (playerCount <= 0)
    {
        return false;
    }

    // build the players linked list
    PlayerNode* playersHead = nullptr;

    for (int i = 0; i < playerCount; i++) {
        if (playerIDs[i].empty()) continue;

        // to avoid duplicates
        if (playerExistsInList(playersHead, playerIDs[i])) continue;

        PlayerNode* newPlayer = new PlayerNode;
        newPlayer->userID = playerIDs[i];
        newPlayer->next = playersHead;
        playersHead = newPlayer;
    }

    // ensure winner is one of the players mentioned
    if (!playerExistsInList(playersHead, winnerID)) {
        // to clean the allocated nodes before it fails
        PlayerNode* p = playersHead;
        while (p != nullptr) {
            PlayerNode* temp = p;
            p = p->next;
            delete temp;
        }
        return false;
    }

    // creating and storing the record
    PlayRecord* record = new PlayRecord;
    record->playID = generatePlayID();
    record->gameID = gameID;
    record->gameName = gameName;
    record->winnerID = winnerID;
    record->players = playersHead;
    record->next = first;
    first = record;

    cout << "Play recorded. PlayID = " << record->playID << "\n";
    return true;
}

/*
Print all recorded plays
pre: none
post: recorded plays are displayed
input params: users- Reference to UserDictionary for ID
Return value: none
*/
void PlayHistory::printAll(UserDictionary& users) {
    if (first == nullptr) {
        cout << "No play records.\n";
        return;
    }

    PlayRecord* current = first;
    while (current != nullptr) {
        cout << "PlayID: " << current->playID << "\n";
        cout << "Game: " << current->gameID << " - " << current->gameName << "\n";

        // Displaying WinnerID + name
        User* w = users.getUser(current->winnerID);
        if (w != nullptr) {
            cout << "Winner: " << current->winnerID << " (" << w->getUserName() << ")\n";
        }
        else {
            cout << "Winner: " << current->winnerID << " (Unknown)\n";
        }

        // to display all players
        cout << "Players: ";
        PlayerNode* p = current->players;
        while (p != nullptr) {
            User* u = users.getUser(p->userID);
            if (u != nullptr) {
                cout << p->userID << " (" << u->getUserName() << ")";
            }
            else {
                cout << p->userID << " (Unknown)";
            }

            if (p->next != nullptr) cout << ", ";
            p = p->next;
        }
        cout << "\n--------------------------\n";

        current = current->next;
    }
}

/*
Print all recorded plays after sorting them by game name
pre: none
post: recorded plays are displayed after sorting
input params: users- Reference to UserDictionary for ID
Return value: none
*/
void PlayHistory::printSortedByGame(UserDictionary& users) {
    if (first == nullptr) {
        cout << "No play records.\n";
        return;
    }

    // Counting the records
    int count = 0;
    PlayRecord* current = first;
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    // Copying pointers into an array
    PlayRecord* records = new PlayRecord[count];
    current = first;
    for (int i = 0; i < count; i++) {
        records[i] = *current;   // copy the play record
        current = current->next;
    }

    // Selection sort by gameName (ascending)
    for (int i = 0; i < count - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < count; j++) {
            if (records[j].gameName < records[minIndex].gameName) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            PlayRecord temp = records[i];
            records[i] = records[minIndex];
            records[minIndex] = temp;
        }
    }

    // Printing in sorted order
    for (int i = 0; i < count; i++) {
        cout << "PlayID: " << records[i].playID << "\n";
        cout << "Game: " << records[i].gameID
            << " - " << records[i].gameName << "\n";

        // Winner
        User* w = users.getUser(records[i].winnerID);
        if (w != nullptr) {
            cout << "Winner: " << records[i].winnerID
                << " (" << w->getUserName() << ")\n";
        }
        else {
            cout << "Winner: " << records[i].winnerID << " (Unknown)\n";
        }

        // Players
        cout << "Players: ";
        PlayerNode* p = records[i].players;
        while (p != nullptr) {
            User* u = users.getUser(p->userID);
            if (u != nullptr) {
                cout << p->userID << " (" << u->getUserName() << ")";
            }
            else {
                cout << p->userID << " (Unknown)";
            }

            if (p->next != nullptr) cout << ", ";
            p = p->next;
        }

        cout << "\n--------------------------\n";
    }
    delete[] records;
}