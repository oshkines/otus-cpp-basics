#include <iostream>
#include <string>
#include <vector>
#include "statistics.hpp"
#include "random_shuffle.hpp"
#include "SortAlgorithmMeasuring.hpp"

using namespace std;

/* void fill_vector(std::vector<int> &values, size_t count) {
	values.resize(count);
	for (size_t i = 0; i < count; ++i) {
		values[i] = static_cast<int>(i);
	}
} */

/*     enum TypeSort{
        EnumSortAlgorithmMeasuring,
        EnumQuickSortAlgorithmMeasuring
    }; */

int main(){

    setlocale(LC_ALL, "ru");

    int value, iteration_count;
    vector<int> VectorVvoda;

    cout << "Enter the number of iterations: ";
    cin >> iteration_count;
    // Handle invalid input data
	if (!cin.eof() && !cin.good()) {
		cerr << "Incorrect data entered!\n";
		return 1;
	}    

    cout << endl << "Enter the numbers in the sequence, after entering each new number press Enter,\n to stop entering Ctrl+Z and Enter" << endl;

    while (cin >> value)
    {
        // Handle invalid input data
	    if (!cin.eof() && !cin.good()) {
		    cerr << "Incorrect data entered!\n";
		    return 1;
	    }
        VectorVvoda.push_back(value);
    }

    //fill_vector(VectorVvoda, iteration_count);

    RandomVector(VectorVvoda);

    cout << "iteration_count = " << iteration_count << endl;

    TypeSort ts {TypeSort::EnumSortAlgorithmMeasuring};
    //for (int i = 0; i < iteration_count; i++)
    //{
        cout << "First sorting option:" << endl;
        	// Print results if any
	    for (size_t i = 0; i < VectorVvoda.size(); ++i) {
		    cout << VectorVvoda[i]  << " ";
	    }
        
        vector<double> VectorForStatistics1 = SortAlgorithmMeasuring(VectorVvoda, iteration_count, ts);
        int Result1 = RanStatistics(VectorForStatistics1);
    //}

    RandomVector(VectorVvoda);
    TypeSort ts1 {TypeSort::EnumQuickSortAlgorithmMeasuring};
    //for (int i = 0; i < iteration_count; i++)
    //{
        cout << endl << endl << "Second sorting option:" << endl;
        vector<double> VectorForStatistics2 = SortAlgorithmMeasuring(VectorVvoda, iteration_count, ts1);
        int Result2 = RanStatistics(VectorForStatistics2);
    //}    


    VectorVvoda.erase(next(VectorVvoda.begin()));
    VectorForStatistics1.erase(next(VectorForStatistics1.begin()));
    VectorForStatistics2.erase(next(VectorForStatistics2.begin()));

    int Result;
    Result1 == 0 && Result2 == 0 ? Result = 0 : Result = 1;

    return Result;

}