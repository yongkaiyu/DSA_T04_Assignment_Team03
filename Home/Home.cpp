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

void adminMenu(Admin& admin, UserDictionary& users, GameDictionary& games, Booking& booking) {
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
        cin.ignore(); // clear newline

        if (choice == 1) {
            Game g;
            cout << "Game name: ";
            getline(cin, g.gameName);

            cout << "Min players: ";
            cin >> g.gameMinPlayer;
            cout << "Max players: ";
            cin >> g.gameMaxPlayer;
            cout << "Min play time: ";
            cin >> g.gameMinPlayTime;
            cout << "Max play time: ";
            cin >> g.gameMaxPlayTime;
            cout << "Year published: ";
            cin >> g.gameYearPublished;
            cin.ignore();

            // optional validation
            if (g.gameMinPlayer > g.gameMaxPlayer || g.gameMinPlayTime > g.gameMaxPlayTime) {
                cout << "Invalid min/max values.\n";
                continue;
            }

            games.addOrUpdateGame(g);
            cout << "Game added/updated successfully.\n";
        }
        else if (choice == 2) {
            string name;
            cout << "Enter game name to remove: ";
            getline(cin, name);

            // This requires teammate to implement removeByName
            // bool ok = games.removeByName(name);

            cout << "Remove game is pending GameDictionary::removeByName().\n";
        }
        else if (choice == 3) {
            string memberID, memberName;
            cout << "Member ID: ";
            getline(cin, memberID);
            cout << "Member Name: ";
            getline(cin, memberName);

            bool ok = admin.addMember(users, memberID, memberName);
            cout << (ok ? "Member added.\n" : "Failed to add member (duplicate/invalid).\n");
        }
        else if (choice == 4) {
            booking.printAdminSummary();
        }
        else if (choice == 0) {
            cout << "Exiting admin menu...\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    GameDictionary lib;

	loadGamesFromCSV("games.csv", lib); // Load data from CSV into the dictionary

    //std::cout << "Welcome to NPTTGC Management System" << std::endl;
    UserDictionary users;
    GameDictionary games;
    Booking booking;

    Admin admin("A001", "Admin");

    adminMenu(admin, users, games, booking);
    
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
