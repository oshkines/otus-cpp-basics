#include <iostream> 
#include <vector> 
#include <list> 

using namespace std;

template <typename Container>
void Send_message(Container &container){
    for (auto element : container)
    {
        cout << element;// << ", ";
        if (element != container.back()){
          cout <<  ", ";  
        }
    }
    cout << endl << "size: " << size(container) << endl << endl;
}

template <typename Container>
void deleteElements(Container& container) {

    int ArrDelete[] = { 3, 5, 7 };
    int counter = 0;

    for (int j = 0; j < size(ArrDelete); j++)
    {
        int i = 0;
        for (auto it = container.begin(); it != container.end(); ++it)
        {
            i++;
            if (ArrDelete[j] - counter  == i)
            {
                container.erase(it);
                break;
            }
        }
        counter++;
     }
}

template <typename Container>
void addElements(Container &container){
    //добавляем элементы в контейнер
    cout << "Add elements to the collection" << endl;

    container.insert(container.begin(), 10);
    Send_message(container);

    int place = size(container)/2;

    int i = 0;
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        if (place == i)
        {
            container.insert(it, 20);
            break;
        }
        i++;
    }
    Send_message(container);

    container.push_back(30);
    Send_message(container);
}

template <typename Container>
void Starting(Container &container)
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

    cout << "Working with container vector" << endl;
    vector<int> Vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Starting(Vector);

    cout << "Working with container list" << endl;
    list<int> List{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    Starting(List);

    return 0;
}
