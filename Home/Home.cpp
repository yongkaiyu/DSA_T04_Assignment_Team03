// Home.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Games.h"
#include "CSVReader.h" // Include your new utility
#include "Admin.h"
#include "UserDictionary.h"
#include "User.h"
#include "Member.h"
#include "Booking.h"

using namespace std;

void adminMenu(Admin admin, UserDictionary users, GameDictionary& lib, Booking* bookingSystem) {
    int choice = -1;

    while (choice != 0) {
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
        }
        else if (choice == 3) {
            // ask for username only (userid not required as its auto generated)
            cout << "Enter username: ";
            string userName;
            cin >> userName;

            // add member
            bool success = admin.addMember(users, userName);

            if (success) {
                cout << "Member created successfully!\n";
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
    }
}

int main() {
    GameDictionary lib;
	Booking* bookingSystem = new Booking;

    UserDictionary users;
    Admin admin("A001", "Admin");

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
        if (users.contains(memberID)) {
            memberMenu(bookingSystem, memberID);
        }
        else {
            cout << "Member ID not found.\n";
        }
    }
    else
    {
        cout << "Invalid role selection.\n";
	}

	//loadGamesFromCSV("games.csv", lib); // Load data from CSV into the dictionary

    //std::cout << "Welcome to NPTTGC Management System" << std::endl;
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
