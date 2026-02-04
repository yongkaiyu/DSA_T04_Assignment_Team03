#include "PlayHistory.h"
#include "UserDictionary.h"
using namespace std;

PlayHistory::PlayHistory() {
    head = nullptr;
    playCount = 0;
}

PlayHistory::~PlayHistory() {
    PlayRecord* current = head;
    while (current != nullptr) {
        // delete players list
        PlayerNode* p = current->players;
        while (p != nullptr) {
            PlayerNode* tempP = p;
            p = p->next;
            delete tempP;
        }

        // delete record node
        PlayRecord* tempR = current;
        current = current->next;
        delete tempR;
    }
    head = nullptr;
}

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

bool PlayHistory::playerExistsInList(PlayerNode* list, string userID) {
    PlayerNode* cur = list;
    while (cur != nullptr) {
        if (cur->userID == userID) return true;
        cur = cur->next;
    }
    return false;
}

bool PlayHistory::addPlay(string gameID, string gameName, string winnerID, string playerIDs[], int playerCount) {
    if (gameID.empty() || winnerID.empty()) return false;
    if (playerCount <= 0) return false;

    // build players list
    PlayerNode* playersHead = nullptr;

    for (int i = 0; i < playerCount; i++) {
        if (playerIDs[i].empty()) continue;

        // avoid duplicates
        if (playerExistsInList(playersHead, playerIDs[i])) continue;

        PlayerNode* newP = new PlayerNode;
        newP->userID = playerIDs[i];
        newP->next = playersHead;
        playersHead = newP;
    }

    // ensure winner is in players list
    if (!playerExistsInList(playersHead, winnerID)) {
        // cleanup before returning false
        PlayerNode* p = playersHead;
        while (p != nullptr) {
            PlayerNode* temp = p;
            p = p->next;
            delete temp;
        }
        return false;
    }

    // create record
    PlayRecord* rec = new PlayRecord;
    rec->playID = generatePlayID();
    rec->gameID = gameID;
    rec->gameName = gameName;
    rec->winnerID = winnerID;
    rec->players = playersHead;
    rec->next = head;
    head = rec;

    cout << "Play recorded. PlayID = " << rec->playID << "\n";
    return true;
}

void PlayHistory::printAll(UserDictionary& users) {
    if (head == nullptr) {
        cout << "No play records.\n";
        return;
    }

    PlayRecord* cur = head;
    while (cur != nullptr) {
        cout << "PlayID: " << cur->playID << "\n";
        cout << "Game: " << cur->gameID << " - " << cur->gameName << "\n";

        // Winner: ID + name
        User* w = users.getUser(cur->winnerID);
        if (w != nullptr) {
            cout << "Winner: " << cur->winnerID << " (" << w->getUserName() << ")\n";
        }
        else {
            cout << "Winner: " << cur->winnerID << " (Unknown)\n";
        }

        cout << "Players: ";
        PlayerNode* p = cur->players;
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

        cur = cur->next;
    }
}

void PlayHistory::printByUserID(string userID, UserDictionary& users) {
    if (head == nullptr) {
        cout << "No play records.\n";
        return;
    }

    int count = 0;
    PlayRecord* cur = head;
    while (cur != nullptr) {
        if (playerExistsInList(cur->players, userID)) {
            count++;
            cout << "PlayID: " << cur->playID
                << " | GameID: " << cur->gameID
                << " | Winner: " << cur->winnerID << "\n";
        }
        cur = cur->next;
    }

    if (count == 0) cout << "No plays found for user " << userID << ".\n";
}

void PlayHistory::printByGameID(string gameID, UserDictionary& users) {
    if (head == nullptr) {
        cout << "No play records.\n";
        return;
    }

    int count = 0;
    PlayRecord* cur = head;
    while (cur != nullptr) {
        if (cur->gameID == gameID) {
            count++;
            cout << "PlayID: " << cur->playID
                << " | Winner: " << cur->winnerID << " | Players: ";

            PlayerNode* p = cur->players;
            while (p != nullptr) {
                cout << p->userID;
                if (p->next != nullptr) cout << ", ";
                p = p->next;
            }
            cout << "\n";
        }
        cur = cur->next;
    }

    if (count == 0) cout << "No plays found for game " << gameID << ".\n";
}