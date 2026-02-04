#include "PlayHistory.h"
#include "UserDictionary.h"
using namespace std;

PlayHistory::PlayHistory() {
    first = nullptr;
    playCount = 0; // initialises it to empty play history list
}

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

// to generate the unique playid
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

// checks if userid already exists in linked list, to prevent duplicates
bool PlayHistory::playerExistsInList(PlayerNode* list, string userID) {
    PlayerNode* current = list;
    while (current != nullptr) {
        if (current->userID == userID) return true;
        current = current->next;
    }
    return false;
}

// records the play of a game of a user
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

// to display the play recorded by user after entering all the information
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