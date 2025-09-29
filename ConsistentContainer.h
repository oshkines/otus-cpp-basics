#pragma once

template<typename T>
class ConsistentContainer
{
private:
    int size{};
    int primarySize{};
     T* data;
public:
    ConsistentContainer(int size, T* arr);
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
ConsistentContainer<T>::ConsistentContainer(int size, T *arr)
{
    this -> size = size;
    this -> primarySize = size;
    data = new T[size];
    for (int i = 0; i < size; i++)
    {
        data[i] = arr[i];
    }
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
        T* new_Data = new T[size + 1];
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
        if (i >= index && i != size - 1)
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
    T* new_Data = new T[size + 1];
    int counter = 0;
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
    size++;
}

template <typename T>
T& ConsistentContainer<T>::operator[](const int index)
{
    return this->data[index];
}
