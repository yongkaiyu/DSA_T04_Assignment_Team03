#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include "Games.h"

// This function will handle all the file reading logic
void loadGamesFromCSV(std::string filename, GameDictionary& dict);

