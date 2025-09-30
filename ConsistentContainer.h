#pragma once

template<typename T>
class ConsistentContainer
{
private:
    int size{};
    int primarySize{};
     T* data;
public:
    ConsistentContainer(int size);
    ~ConsistentContainer();
    void push_back(T value);
    void clear();
    //void pop_front();
    int getSize() { return size; }
    void removeAt(int index);
    void insert(T value, int index);
    T& operator [] (const int index);
};

template<typename T>
ConsistentContainer<T>::ConsistentContainer(int size)
{
    this -> size = 0;
    this -> primarySize = size;
    this -> data = new T[size];

}

template<typename T>
ConsistentContainer<T>::~ConsistentContainer()
{
    clear();
}

template <typename T>
void ConsistentContainer<T>::clear()
{
    delete[] data;
    size = 0;
    data = nullptr;
}

template <typename T>
void ConsistentContainer<T>::push_back(T value)
{
    if (primarySize >= size)
    {
        data[size] = value;
    }
    else
    {
        primarySize = 2 * size;
        T* new_Data = new T[primarySize];

        for (size_t i = 0; i < size; ++i) {
            new_Data[i] = data[i];
        }
        new_Data[size] = value;
        delete[] data;
        data = new_Data;
    }
    size++;
}

template <typename T>
void ConsistentContainer<T>::removeAt(int index)
{
    int deletedElement{};
    //T* new_Data = new T[size - 1];
    int counter = 0;
    for (size_t i = 0; i < size; ++i) { 
        if (i == index) {
            deletedElement = data[i];
            counter++;
            data[i] = data[i + counter];
        }
        else if (i >= index && i != size - 1)
        {
            data[i] = data[i + counter];
             //continue;
        }
        else if (i == size - 1)
        {
            data[i] = deletedElement;
        }

        //new_Data[i - counter] = data[i];
    }
    //delete[] data;
    //data = new_Data;
    size--;
}

template <typename T>
void ConsistentContainer<T>::insert(T value, int index)
{
    int counter = 0;
    if (primarySize >= size) {
        T tmp{};
        for (size_t i = 0; i < size + 1; ++i) {
            if (i == index)
            {
                //counter++;
                //tmp = data[i + counter];
                //data[i + counter] = data[i];
                //data[i] = value;
                tmp = data[i];
                data[i] = value;
            }
            else if (i < index)
            {
               continue;
            }
            else
            {
                //T tmp2 = data[i + counter];
                //data[i + counter] = data[i];
                //tmp = tmp2;
                T tmp2 = data[i];
                data[i] = tmp;
                tmp = tmp2;
            }
        }

    }
    else
    {
        T* new_Data = new T[size + 1];
        for (size_t i = 0; i < size; ++i) {
            if (i == index)
            {
                counter++;
                new_Data[i] = value;
                new_Data[i + counter] = data[i];
            }
            else if (i < index)
            {
                new_Data[i] = data[i];
            }
            else
            {
                new_Data[i + counter] = data[i];
            }
        }
        delete[] data;
        data = new_Data;
    }
    size++;

}

template <typename T>
T& ConsistentContainer<T>::operator[](const int index)
{
    return this->data[index];
}
