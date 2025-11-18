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
    bool emptry(){ return size == 0; }    
    void removeAt(int index);
    void insert(T value, int index);
    T& operator [] (const int index);
    void push_front(T data);
};

template<typename T>
ConsistentContainer<T>::ConsistentContainer(int size)
{
    this -> size = 0;
    this -> primarySize = size * 1.4;
    //T* data = new T[primarySize];
    this -> data = new T[primarySize];
    for (size_t i = 0; i < primarySize; ++i) {
        data[i] = 0;
    }
}

template<typename T>
ConsistentContainer<T>::~ConsistentContainer()
{
    if (size != 0) { delete[] data; }
    size = 0;
    data = nullptr;
}

template <typename T>
void ConsistentContainer<T>::clear()
{
    for (size_t i = 0; i < size; ++i) {
        data[i] = 0;
    }
    size = 0;
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
        primarySize = size*1.4;
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

template<typename T>
void ConsistentContainer<T>::push_front(T value)
{
    if (primarySize <= size)
    {
        primarySize = size * 1.4;
        T* new_Data = new T[primarySize];

        for (size_t i = 0; i < size; ++i) {
            new_Data[i] = data[i];
        }
        delete[] data;
        data = new_Data;
    }
    T tmp{ value };
    T tmp1{};
    for (size_t i = 0; i < size + 1; ++i) {
        tmp1 = data[i];
        data[i] = tmp;
        tmp = tmp1;
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
    T tmp{ value };

    if (primarySize <= size)
    {
        primarySize = size * 1.4;
        T* new_Data = new T[primarySize];

        for (size_t i = 0; i < size; ++i) {
            new_Data[i] = data[i];
        }
        delete[] data;
        data = new_Data;
    }

    if (index == 0) {
        push_front(value);
    }
    else{ //if (primarySize >= size) {
        //T tmp{};
        for (size_t i = 0; i < size + 1; ++i) {
            //if (i == index)
            //{
            //    //counter++;
            //    //tmp = data[i + counter];
            //    //data[i + counter] = data[i];
            //    //data[i] = value;
            //    tmp = data[i];
            //    data[i] = value;
            //}
            //else 
            if (i < index)
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
        size++;
    }
    /*else
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
    }*/
    //size++;

}

template <typename T>
T& ConsistentContainer<T>::operator[](const int index)
{
    return this->data[index];
}
