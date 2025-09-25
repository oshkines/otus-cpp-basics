#pragma once

#ifndef ConsistentContainer_H  
#define ConsistentContainer_H

using namespace std;

template<typename T>
class ConsistentContainer
{
private:
    int Size;
public:
    T* Data;
    ConsistentContainer(/* args */);
    ~ConsistentContainer();
    void push_back(T value);
    void Clear();
    //void pop_front();
    int getSize() { return Size; }
    void remuveAT(int index);
    void insert(T value, int index);
    T& operator [] (const int index);
};

template<typename T>
ConsistentContainer<T>::ConsistentContainer(/* args */)
{
    Size = 0;
    Data = new T[0];//T Data[0];
}

template<typename T>
ConsistentContainer<T>::~ConsistentContainer()
{
    Clear();
}

template <typename T>
void ConsistentContainer<T>::Clear()
{
    delete[] Data;
}

template <typename T>
void ConsistentContainer<T>::push_back(T value)
{
    T* new_Data = new T[Size + 1];
    for (size_t i = 0; i < Size; ++i) {
        new_Data[i] = Data[i];
    }
    new_Data[Size] = value;
    delete[] Data;
    Data = new_Data;
    Size++;
}

template <typename T>
void ConsistentContainer<T>::remuveAT(int index)
{
    T* new_Data = new T[Size - 1];
    int counter = 0;
    for (size_t i = 0; i < Size; ++i) {
        if (i == index)
        {
            counter++;
            continue;
        }

        new_Data[i - counter] = Data[i];
    }
    delete[] Data;
    Data = new_Data;
    Size--;
}

template <typename T>
void ConsistentContainer<T>::insert(T value, int index)
{
    T* new_Data = new T[Size + 1];
    int counter = 0;
    for (size_t i = 0; i < Size; ++i) {
        if (i == index)
        {
            counter++;
            new_Data[i] = value;
            new_Data[i + counter] = Data[i];
        }
        else if (i < index)
        {
            new_Data[i] = Data[i];
        }
        else
        {
            new_Data[i + counter] = Data[i];
        }
    }
    delete[] Data;
    Data = new_Data;
    Size++;
}

template <typename T>
T& ConsistentContainer<T>::operator[](const int index)
{
    return this->Data[index];
}

#endif