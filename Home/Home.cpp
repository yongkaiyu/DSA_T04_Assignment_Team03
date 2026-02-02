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
			bookingSystem->printAdminSummary();
        }
        else {
            break;
        }
    }
}

void memberMenu(Booking* bookingSystem, string userID) {
    int choice = -1;

    while (choice != 0) {
        cout << "\n=== MEMBER MENU ===\n";
        cout << "1) Borrow a board game\n";
        cout << "2) Return a board game\n";
        cout << "3) Display summary of games borrowed/returned\n";
		cout << "4) Rate a board game\n";
        cout << "0) Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            // borrow game (booking)
			string gameID;
			cout << "Enter the game ID to borrow: ";
			cin >> gameID;
			bookingSystem->borrowGame(userID,gameID);
			cout << "Game borrowed successfully.\n";
        }
        else if (choice == 2) {
            // return game (booking)
			string bookingID;
			cout << "Enter the booking ID to return: ";
			cin >> bookingID;
			bookingSystem->returnGame(bookingID);
			cout << "Game returned successfully.\n";
        }
        else if (choice == 3) {
            // display summary of games borrowed/returned (booking and users maybe)
			bookingSystem->printMemberSummary(userID);
        }

        else if (choice == 4) {
            string gameID;
            float rating;

            cout << "Enter the game ID to rate: ";
            cin >> gameID;
            cout << "Enter your rating (1-10): ";
            cin >> rating;

            float newAvg = gameDict->rateGame(gameID, rating);

            if (newAvg != -1.0f) {
                // We use fixed and setprecision(1) to make it look like "8.5" instead of "8.5000"
                cout << "Game rated successfully. New Average Rating: " << newAvg << "/10\n";
            }
            else {
                cout << "Error: Invalid ID or rating score.\n";
            }
        }
    }
}

int main() {
    GameDictionary lib;
	Booking* bookingSystem = new Booking;

    UserDictionary users;
    Admin admin("A0001", "Admin");

    while (true) {
        int roleChoice = -1;
        cout << "Select Role:\n1) Admin\n2) Member\nChoose: ";
        cin >> roleChoice;
        if (roleChoice == 1)
        {
            adminMenu(admin, users, lib, bookingSystem);
        }
        else if (roleChoice == 2)
        {
            string memberID;
            cout << "Enter Member ID: ";
            cin >> memberID;
            cout << memberID;
            if (users.contains(memberID)) {
                memberMenu(bookingSystem, memberID);
            }
            else {
                cout << "Member ID not found.\n";
                cout << users.getUser(memberID);
            }
        }
        else if (roleChoice == 0) {
            break;
        }
        else
        {
            cout << "Invalid role selection.\n";
        }
    }

	

	//loadGamesFromCSV("games.csv", lib); // Load data from CSV into the dictionary

    //std::cout << "Welcome to NPTTGC Management System" << std::endl;
    
    return 0;
}
