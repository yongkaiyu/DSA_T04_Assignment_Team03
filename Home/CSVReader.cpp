//TO POPULATE THE DICTIONARY WITH DATA FROM games.csv

#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

void loadGamesFromCSV(std::string filename, GameDictionary& dict) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not find " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip the header row

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fields[7]; // name, min, max, maxplay, minplay, year, rating
        std::string cell;
        int i = 0;

        while (std::getline(ss, cell, ',') && i < 7) {
            fields[i++] = cell;
        }

        if (i < 5) continue; // Skip incomplete lines

        Game g;
        g.gameName = fields[0];
        g.gameMinPlayer = std::stoi(fields[1]);
        g.gameMaxPlayer = std::stoi(fields[2]);
        g.gameMaxPlayTime = std::stoi(fields[3]);
        g.gameMinPlayTime = std::stoi(fields[4]);
        g.gameYearPublished = std::stoi(fields[5]);

        g.gameAverageRating = 0.0f; // Initialize to 0 since it's not in the CSV

        // Note: gameID and copy counts are handled inside addOrUpdateGame
        dict.addOrUpdateGame(g);
    }
    file.close();
}