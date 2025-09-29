
	#include <iostream>
	#include "ConsistentContainer.h"
	#include "listContainer.h"

	using namespace std;

	template <typename Container>
	void Send_message(Container& container) {
		for (int i = 0; i < container.getSize(); ++i)
		{
			cout << container[i];// << ", ";
			if (i+1 != container.getSize()) {
				cout << ", ";
			}
		}
		cout << endl << "size: " << container.getSize() << endl << endl;
	}

	template <typename Container>
	void deleteElements(Container& container) {

		int ArrDelete[] = { 3, 5, 7 };
		int counter = 0;

		for (int j = 2; j >= 0; j--)
		{
			container.removeAt(ArrDelete[j] - 1);
		}
	} 

	template <typename Container>
	void addElements(Container& container) {

		cout << "Add elements to the collection" << endl;

		container.insert(10, 0);
		Send_message(container);

		int place = container.getSize() / 2;
		container.insert(20, place);

		Send_message(container);

		container.push_back(30);
		Send_message(container);
	}

	template <typename Container>
	void Starting(Container& container)
	{

		cout << "We output the contents of the array to the console" << endl;
		Send_message(container);

		cout << "Delete Elements from container" << endl;
		deleteElements(container);

		Send_message(container);

		addElements(container);

	}

	int main()
	{

		setlocale(LC_ALL, "Russian");

		cout << "Working with consistent container" << endl;

		int arr[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		int size{10};

		ConsistentContainer<int> cc{ size, arr };
		//cc.push_back(0);
		//cc.push_back(1);
		//cc.push_back(2);
		//cc.push_back(3);
		//cc.push_back(4);
		//cc.push_back(5);
		//cc.push_back(6);
		//cc.push_back(7);
		//cc.push_back(8);
		//cc.push_back(9);
		Starting(cc);

		cout << "Working with list container" << endl;

		listContainer<int> lc{ size, arr };
		//lc.push_back(0);
		//lc.push_back(1);
		//lc.push_back(2);
		//lc.push_back(3);
		//lc.push_back(4);
		//lc.push_back(5);
		//lc.push_back(6);
		//lc.push_back(7);
		//lc.push_back(8);
		//lc.push_back(9);


		Starting(lc);

		return 0;
	}
