#include <iostream>
#include <string>
#include "getRandomNumber.hpp"
#include "SaveFile.hpp"

int main(){
    using namespace std; 

    string user_name;
    int TempValue = 0;
    TempValue = getRandomNumber();
    cout << TempValue << endl;
    const int getRandomNumber = TempValue; 
    //delete TempValue;
	int current_value = 0;
	bool not_win = true;
    int attempts_count = 0;

 
    cout << "Hi! Enter your name, please:" << endl;
    cin >> user_name;

    cout << "Enter your high score:" << endl;

	do {

        attempts_count = attempts_count++;
        cin >> current_value;

		if (current_value < getRandomNumber) {
			cout << "less than " << current_value << endl;
		}
		else if (current_value > getRandomNumber) {
			cout << "greater than " << current_value << endl;
		}
		else {
			cout << "you win!" << endl;
			break;
		}

	} while(true);

	return SaveFile(user_name, attempts_count);
}
