#include <chrono>
#include <iostream>
#include <vector>
#include "random_shuffle.hpp"
#include "SortAlgorithmMeasuring.hpp"

#include <algorithm> 

using namespace std;

    void fill_vector(std::vector<int> &values, size_t count) {
	    values.resize(count);
	    for (size_t i = 0; i < count; ++i) {
		values[i] = static_cast<int>(i);
	    }
    }

    class Sort{
        public:
        size_t iteration_count;
        vector<double> Measurements;
        virtual void SortAlgorithmMeasuring(std::vector<int> &VectorVvoda) = 0;
    };

    class Sort1 : public Sort { 
        public:
            Sort1(size_t iteration_count){
                this ->iteration_count = iteration_count;
            }        
            virtual void SortAlgorithmMeasuring(std::vector<int> &VectorVvoda) override 
            { 
                const size_t elements_count = VectorVvoda.size();
                //std::vector<int> values;
                // fill values with elements_count elements
                fill_vector(VectorVvoda, elements_count);
                // random shuffle values
                RandomVector(VectorVvoda);

                //std::cout << "iteration_count = " << iteration_count << std::endl;
                // std::sort algorithm measuring
                {

                    for (size_t i = 0; i < iteration_count; ++i) {
                        // Take a start time
                        auto start_time = std::chrono::system_clock::now();
                        // Make a several iterations                    
                        std::vector<int> test_data = VectorVvoda;
                        std::sort(test_data.begin(), test_data.end());
                        // Take an end time
                        auto end_time = std::chrono::system_clock::now();
                        // Calculate a duration
                        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                            end_time - start_time);  
                        Measurements.push_back(milliseconds.count() / iteration_count);                                          
                    }
                }
            }
    };



    class Sort2 : public Sort { 
        public:
           Sort2(int iteration_count){
                this ->iteration_count = iteration_count;
            }         
            virtual void SortAlgorithmMeasuring(std::vector<int> &VectorVvoda) override { 
                const size_t elements_count = VectorVvoda.size();
                //std::vector<int> values;
                // fill values with elements_count elements
                fill_vector(VectorVvoda, elements_count);
                // random shuffle values
                RandomVector(VectorVvoda);            
                // Take a start time

                for (size_t i = 0; i < iteration_count; ++i) {
                    auto start_time = std::chrono::system_clock::now();
                    // Make a several iterations                
                    std::vector<int> test_data = VectorVvoda;
                    std::qsort(
                        test_data.data(),
                        test_data.size(),
                        sizeof(int),
                        [](const void *x, const void *y) {
                            const int arg1 = *static_cast<const int *>(x);
                            const int arg2 = *static_cast<const int *>(y);
                            if (arg1 < arg2)
                                return -1;
                            if (arg1 > arg2)
                                return 1;
                            return 0;
                        });
                    // Take an end time
                    auto end_time = std::chrono::system_clock::now();
                    // Calculate a duration
                    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time);  
                    Measurements.push_back(milliseconds.count() / iteration_count);                                       
                }
            }
    };   

    //enum TypeSort{
    //EnumSortAlgorithmMeasuring,
    //EnumQuickSortAlgorithmMeasuring
    //};

    class Starting{
        public:
         void SortAlgorithmMeasuring(Sort &sorting, std::vector<int> &VectorVvoda){
            sorting.SortAlgorithmMeasuring(VectorVvoda);
         }
    };

    std::vector<double> SortAlgorithmMeasuring(std::vector<int> &VectorVvoda, int iteration_count, TypeSort ts){

            if(ts == TypeSort::EnumSortAlgorithmMeasuring){ 
                Starting starting;
                Sort1 sort1(iteration_count);
                starting.SortAlgorithmMeasuring(sort1, VectorVvoda);
                return sort1.Measurements;
            }    
            else //if (ts == TypeSort::EnumQuickSortAlgorithmMeasuring)
            {
                Starting starting;
                Sort2 sort2(iteration_count);
                starting.SortAlgorithmMeasuring(sort2, VectorVvoda); 
                return sort2.Measurements; 
            }
    }
