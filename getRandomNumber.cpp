#include <iostream>
#include <cstdlib>
#include <ctime>
#include "getRandomNumber.hpp"

int getRandomNumber() {

	const int max_value = 100;

	std::srand(std::time(nullptr)); // use current time as seed for random generator

	const int random_value = std::rand() % 100;

	//std::cout << random_value << std::endl;
    //std::cout << 32767 % 100 << std::endl;

	return random_value;
}