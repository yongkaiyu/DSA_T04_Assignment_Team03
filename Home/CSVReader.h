#pragma once

#include <string>
#include "Games.h"

// This function will handle all the file reading logic
//void loadGamesFromCSV(std::string& filename, GameDictionary& dict);

// Add 'const' before std::string
void loadGamesFromCSV(const std::string& filename, GameDictionary& dict);

