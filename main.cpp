#include <iostream>
#include <string>
#include "getRandomNumber.hpp"
#include "SaveFile.hpp"

int main(){
    using namespace std; 

    string user_name;
    int *TempValue = 0;
    *TempValue = getRandomNumber();
    const int getRandomNumber = *TempValue; 
    delete TempValue;
	int current_value = 0;
	bool not_win = true;
    int attempts_count = 0;

 
    cout << "Привет! Как тебя зовут, введи пожалуйста свое имя:" << endl;
    cin >> user_name;

    cout << "Введите свое предпологаемое число:" << endl;

	do {

        attempts_count = attempts_count++;
        cin >> current_value;

		if (current_value < getRandomNumber) {
			cout << "Меньше заданного числа " << current_value << endl;
		}
		else if (current_value > getRandomNumber) {
			cout << "Больше заданного числа " << current_value << endl;
		}
		else {
			cout << "Вы выиграли!" << endl;
			break;
		}

	} while(true);

	return 0;
}
