#pragma once

#include <string>
#include "Games.h"
#include "Booking.h"
#include "ActiveBookingIndex.h"

// This function will handle all the file reading logic
//void loadGamesFromCSV(std::string& filename, GameDictionary& dict);

// Add 'const' before std::string
void loadGamesFromCSV(const std::string& filename, GameDictionary& dict);

bool loadBookingsFromCSV(const std::string& filename, Booking& booking);

bool saveBookingsToCSV(const std::string& filename, Booking& booking);

void rebuildDerivedData(Booking& booking, ActiveBookingIndex& activeIndex, GameDictionary& lib);



