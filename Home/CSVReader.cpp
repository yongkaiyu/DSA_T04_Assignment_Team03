//TO POPULATE THE DICTIONARY WITH DATA FROM games.csv

#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm> // for std::trim logic

// Helper function to remove leading/trailing whitespace
std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return s;
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

void loadGamesFromCSV(const std::string& filename, GameDictionary& dict) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    // 1. Skip the header row
    if (!std::getline(file, line)) return;

    int rowCount = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields;
        std::string cell;
        bool inQuotes = false;

        // Manual parse to handle commas inside quotes
        std::string currentField;
        for (char c : line) {
            if (c == '"') {
                inQuotes = !inQuotes; // Toggle quote state
            }
            else if (c == ',' && !inQuotes) {
                fields.push_back(trim(currentField));
                currentField.clear();
            }
            else {
                currentField += c;
            }
        }
        fields.push_back(trim(currentField)); // Add the last field

        if (fields.size() < 6) continue;

        try {
            Game g;
            // 3. Map CSV columns to Game struct members
            g.gameName = fields[0];
            g.gameMinPlayer = std::stoi(fields[1]);
            g.gameMaxPlayer = std::stoi(fields[2]);
            g.gameMaxPlayTime = std::stoi(fields[3]);
            g.gameMinPlayTime = std::stoi(fields[4]);
            g.gameYearPublished = std::stoi(fields[5]);
            g.gameAverageRating = 0.0f; // Default value as it's not in your CSV

            // 4. Add to your Hash Table / Dictionary
            dict.addOrUpdateGame(g);
            rowCount++;

        }
        catch (const std::invalid_argument& e) {
            // This catches the 'abort()' issue if a cell isn't a number
            std::cerr << "Row " << rowCount + 2 << " error: '" << fields[0]
                << "' has non-numeric data." << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Row " << rowCount + 2 << " error: Number too large." << std::endl;
        }
    }

    //std::cout << "Successfully loaded " << rowCount << " games into the dictionary." << std::endl;
    file.close();
}