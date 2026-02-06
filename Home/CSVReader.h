#pragma once

#include <string>
#include "Games.h"

// Added 'const' and '&' to match the .cpp file exactly
void loadGamesFromCSV(const std::string& filename, GameDictionary& dict);
