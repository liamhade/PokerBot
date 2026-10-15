#ifndef INPUT_H
#define INPUT_H

#include "../include/InputHandler.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <functional>

/**
 * @brief In full transparency, this code was largely inspired by code I found
 * through Google's AI overview.
 */
bool is_valid_int(const std::string& input) {
	std::stringstream ss(input);
	int valid_value;

	if (ss >> valid_value) {
		char leftover;
		if (ss >> leftover) {
			return false;
		}
		return true;
	}
	return false;
}

void clear_cin() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif