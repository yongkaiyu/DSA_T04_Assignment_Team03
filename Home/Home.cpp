// Home.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Games.h"
#include "CSVReader.h"
#include "Admin.h"
#include "UserDictionary.h"
#include "User.h"
#include "Member.h"
#include "Booking.h"
#include "ActiveBookingIndex.h"
#include "PlayHistory.h"

using namespace std;

void adminMenu(Admin& admin, UserDictionary& users, GameDictionary& lib, Booking* bookingSystem) {
    int choice = -1;

    while (choice != -2) {
        cout << "\n=== ADMIN MENU ===\n";
        cout << "1) Add a new board game\n";
        cout << "2) Remove a board game\n";
        cout << "3) Add a new member\n";
        cout << "4) Display borrowed/returned summary\n";
        cout << "0) Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            // add new board game (look at game)
            Game g;

            // ask for the details to make a game
            cout << "Enter game name: ";
            cin.ignore();
            getline(cin, g.gameName);

            // minimum players required to play
            cout << "Min players: ";
            cin >> g.gameMinPlayer;

            // maximum players required to play
            cout << "Max players: ";
            cin >> g.gameMaxPlayer;

            // minimum play time for the game (1 hour = 60, etc)
            cout << "Min play time: ";
            cin >> g.gameMinPlayTime;

            // maximum play time for the game (1 hour = 60, etc)
            cout << "Max play time: ";
            cin >> g.gameMaxPlayTime;

            // year published for the game
            cout << "Year published: ";
            cin >> g.gameYearPublished;

            // setting rating to 0 for now (for advanced)
            g.gameAverageRating = 0.0f;

            // checking if minplayer > maxplayer and minplaytime > maxplaytime
            if (g.gameMinPlayer > g.gameMaxPlayer || g.gameMinPlayTime > g.gameMaxPlayTime) {
                cout << "Invalid min/max values.\n";
            }
            else {
                // add game to game dictionary
                lib.addOrUpdateGame(g);
                cout << "Game added successfully!\n";
            }
        }
        else if (choice == 2) {
            // remove a board game (look at game)
            // need remove game to be added first
            cout << "Enter game to be removed: ";
            string gameName;
            cin.ignore();
            getline(cin, gameName);

            int copiesLeft = lib.removeGame(gameName);

            if (copiesLeft == -1) {
                cout << "No such game found.";
            }
            else if (copiesLeft == 0) {
                cout << "Game removed.";
            }
        
        }
        else if (choice == 3) {
            // ask for username only (userid not required as its auto generated)
            cout << "Enter username: ";
            string userName;
            cin >> userName;

            // add member
            string newID;
            bool success = admin.addMember(users, userName, newID);

            if (success) {
                cout << "Member created successfully!\n";
                cout << newID;
            }
            else {
                cout << "Failed to create member";
            }
        }
        else if (choice == 4) {
            // display borrowed/returned summary (look at booking)
			bookingSystem->displayAllSortedByGameID();
        }
        else {
            break;
        }
    }
}

void memberMenu(GameDictionary& lib,Booking* bookingSystem, string userID, ActiveBookingIndex* activeIndex, UserDictionary& users, PlayHistory& plays) {
    int choice = -1;

    while (choice != 0) {
        cout << "\n=== MEMBER MENU ===\n";
        cout << "1) Borrow a board game\n";
        cout << "2) Return a board game\n";
        cout << "3) Display summary of games borrowed/returned\n";
		cout << "4) Rate a board game\n";
        cout << "5) Record a game play\n";
        cout << "6) View my play history\n";
        cout << "7) View play history for a game\n";
        cout << "0) Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
			lib.displayAll();
            // borrow game (booking)
			string gameID;
			cout << "Enter the game ID to borrow: ";
			cin >> gameID;

            if (!lib.gameExists(gameID)) {
				cout << "The game ID does not exist.\n";
                continue;
			}

            else if (activeIndex->hasActive(userID, gameID)) {
                cout << "You have already borrowed this game and not returned it yet.\n";
                continue;
            }

            else if (lib.getAvailableCopiesForGameByID(gameID) == 0)
            {
                cout << "The game has been fully booked.\n";
                continue;
            }
            else if (lib.gameExists(gameID) && lib.getAvailableCopiesForGameByID(gameID) != 0) {

                string bookingID;

                bookingSystem->borrowGame(userID, gameID, bookingID);

                lib.borrowGameUpdateTotalCopies(gameID);

				activeIndex->addActive(userID, gameID, bookingID);
                cout << "Game borrowed successfully. Booking ID: " << bookingID << "\n";
                continue;
            }
            else {
                cout << "An unexpected error occurred.\n";
                continue;
			}
			
        }
        else if (choice == 2) {
            bookingSystem->displaySortedByUserID(userID);
            // return game (booking)
			string bookingID;
			cout << "Enter the booking ID to return: ";
			cin >> bookingID;
            if (!bookingSystem->bookingExists(bookingID)) {
                cout << "The booking ID does not exist.\n";
                continue;
			}
            else if (bookingSystem->isBookingReturned(bookingID)) {
                cout << "The game has already been returned.\n";
                continue;
            }
            else if (!bookingSystem->isUserBookingOwner(bookingID, userID)) {
                cout << "You are not the owner of this booking.\n";
				continue;
            }
            else if (bookingSystem->bookingExists(bookingID) && !bookingSystem->isBookingReturned(bookingID) && bookingSystem->isUserBookingOwner(bookingID,userID)) {

                string gameID = bookingSystem->getGameIDByBookingID(bookingID);

                bookingSystem->returnGame(bookingID);

                lib.returnGameUpdateTotalCopies(gameID);

				activeIndex->removeActive(userID, gameID);
                cout << "Game returned successfully.\n";
                continue;
            }
            else {
                cout << "An unexpected error occurred.\n";
                continue;
            }
			
        }
        else if (choice == 3) {
            // display summary of games borrowed/returned (booking and users maybe)
			bookingSystem->displaySortedByUserID(userID);
        }
        else if (choice == 4) {
            string gameID;
            float rating;

            cout << "Enter the game ID to rate: ";
            cin >> gameID;
            cout << "Enter your rating (1-10): ";
            cin >> rating;

            float newAvg = lib.rateGame(gameID, rating);

            if (newAvg != -1.0f) {
                // We use fixed and setprecision(1) to make it look like "8.5" instead of "8.5000"
                cout << "Game rated successfully. New Average Rating: " << newAvg << "/10\n";
            }
            else {
                cout << "Error: Invalid ID or rating score.\n";
            }
        }
        else if (choice == 5) {
            
            string gameName;
            cout << "Enter game name: ";
            getline(cin >> ws, gameName);

            Game* g = lib.searchByName(gameName);
            if (g == nullptr) {
                cout << "Game not found.\n";
                return;
            }

            //cout << "Selected: " << g->gameID << " - " << g->gameName << "\n";

            int n;
            cout << "How many players? ";
            cin >> n;

            if (n <= 0 || n > 20) {
                cout << "Invalid number of players.\n";
                return;
            }

            string playerIDs[20];

            for (int i = 0; i < n; i++) {
                cout << "Enter Player " << (i + 1) << " MemberID: ";
                cin >> playerIDs[i];

                if (!users.contains(playerIDs[i])) {
                    cout << "MemberID not found: " << playerIDs[i] << "\n";
                    return;
                }
            }

            string winnerID;
            cout << "Enter Winner MemberID: ";
            cin >> winnerID;

            bool ok = plays.addPlay(g->gameID, g->gameName, winnerID, playerIDs, n);
            if (ok){
                cout << "Play recorded successfully!\n";
                cout << "\n";
                cout << "\n---------------------------\n";
                plays.printAll(users);
            }
            else cout << "Failed to record play (winner must be among players).\n";
        }
        else {
			break;
        }

    }
}

//void viewGamesMenu(GameDictionary* gameDict) {
//    int choice = -1;
//    while (choice != 0) {
//        cout << "\n=== VIEW GAMES MENU ===\n";
//        cout << "1) Display details of a particular game\n";
//        cout << "2) List games by number of players\n";
//        cout << "0) Back to Main Menu\n";
//        cout << "Choose: ";
//        cin >> choice;
//
//        if (choice == 1) {
//            string id;
//            cout << "Enter Game ID: ";
//            cin >> id;
//            gameDict->displayGameDetails(id);
//        }
//        else if (choice == 2) {
//            int p;
//            int sortChoice;
//            string sortType = "none";
//
//            cout << "Enter number of players: ";
//            cin >> p;
//            cout << "Sort by: 1) Year of Publication  2) Average Rating  3) None: ";
//            cin >> sortChoice;
//
//            if (sortChoice == 1) sortType = "year";
//            else if (sortChoice == 2) sortType = "rating";
//
//            gameDict->displayFilteredGames(p, sortType);
//        }
//    }
//}

int main() {
    GameDictionary lib;
    Booking bookingSystem;
    ActiveBookingIndex activeIndex;

    UserDictionary users;
    Admin admin("A001", "Admin");

    PlayHistory plays;

    while (true) {
        int roleChoice = -1;
        cout << "Select Role:\n1) Admin\n2) Member\n3) View Games\nChoose: ";
        cin >> roleChoice;
        if (roleChoice == 1)
        {
            string adminID;
            cout << "Enter Admin ID: ";
            cin >> adminID;
            if (adminID == "A001") {
                adminMenu(admin, users, lib, &bookingSystem);
            }
            else {
                cout << "Admin ID not found.\n";
            }
        }
        else if (roleChoice == 2)
        {
            string memberID;
            cout << "Enter Member ID: ";
            cin >> memberID;
            cout << memberID;
            if (users.contains(memberID)) {
                memberMenu(lib,&bookingSystem, memberID, &activeIndex, users,plays);
            }
            else {
                cout << "Member ID not found.\n";
                cout << users.getUser(memberID);
            }
        }

        else if (roleChoice == 3) {
            //viewGamesMenu(&lib);
		}

        else if (roleChoice == 0) {
            break;
        }
        else
        {
            cout << "Invalid role selection.\n";
        }
    }

	loadGamesFromCSV("games.csv", lib); // Load data from CSV into the dictionary

    //std::cout << "Welcome to NPTTGC Management System" << std::endl;
    
    return 0;
}
