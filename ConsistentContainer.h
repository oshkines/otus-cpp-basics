#pragma once

template<typename T>
class ConsistentContainer
{
private:
    int Size;
     T* data;
public:
    ConsistentContainer(/* args */);
    ~ConsistentContainer();
    void push_back(T value);
    void clear();
    //void pop_front();
    int getSize() { return Size; }
    void removeAt(int index);
    void insert(T value, int index);
    T& operator [] (const int index);
};

template<typename T>
ConsistentContainer<T>::ConsistentContainer(/* args */)
{
    Size = 0;
    data = new T[0];//T data[0];
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
}

template <typename T>
void ConsistentContainer<T>::push_back(T value)
{
    T* new_Data = new T[Size + 1];
    for (size_t i = 0; i < Size; ++i) {
        new_Data[i] = data[i];
    }
    new_Data[Size] = value;
    delete[] data;
    data = new_Data;
    Size++;
}

template <typename T>
void ConsistentContainer<T>::removeAt(int index)
{
    T* new_Data = new T[Size - 1];
    int counter = 0;
    for (size_t i = 0; i < Size; ++i) {
        if (i == index)
        {
            counter++;
            continue;
        }

        new_Data[i - counter] = data[i];
    }
    delete[] data;
    data = new_Data;
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
    Size++;
}

template <typename T>
T& ConsistentContainer<T>::operator[](const int index)
{
    return this->data[index];
}
