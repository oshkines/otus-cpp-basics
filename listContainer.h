#pragma once

template<typename T>
class listContainer
{
private:
    int size;
    template<typename T>
    class Node {
    public:
        Node* pNext;
        Node* pPrev;
        T data;
        Node(T data = T(), Node* pNext = nullptr, Node* pPrev = nullptr) {
            this->data = data;
            this->pNext = pNext;
            this->pPrev = pPrev;
        };
    };
    Node<T>* Head;
public:
    listContainer(int size);
    ~listContainer();
    void push_back(T value);
    void clear();
    void pop_front();
    int getSize() { return size; }
    void removeAt(int index);
    void insert(T value, int index);
    T& operator [] (const int index);
    void push_front(T data);
};

template<typename T>
listContainer<T>::listContainer(int size)
{
    Head = nullptr;
    this -> size = 0;
}

template<typename T>
listContainer<T>::~listContainer()
{
    clear();
}


template <typename T>
void listContainer<T>::pop_front()
{

    Node<T>* Temp = Head;
    Head = Head->pNext;
    delete Temp;
    size--;
}

template <typename T>
void listContainer<T>::clear()
{
    while (size) {
        pop_front();
    }
}


template <typename T>
void listContainer<T>::push_back(T data)
{
     if (Head == nullptr)
    {
        Head = new Node<T>(data);
    }
    else
    {
        Node<T>* Current = this->Head; 
        while (Current->pNext != nullptr)
        {
            Current = Current->pNext;
        }
        Current->pNext = new Node<T>(data, nullptr, Current);
    }

     size++;

}


template <typename T>
T& listContainer<T>::operator[](const int index)
{
    int counter = 0; 
    Node<T>* Current = this->Head; 

    while (Current != nullptr) {
        if (counter == index)
        {
            return Current->data;
        }

        Current = Current->pNext;
        counter++;
    }

}

template <typename T>
void listContainer<T>::removeAt(int index)
{
    if (index == 0)
    {
        pop_front();
    }
    else
    {
        Node<T>* previous = this->Head;
        for (int i = 0; i < index - 1; i++)
        {
            previous = previous->pNext; 
        }

        Node<T>* next = this->Head;
        for (int i = 0; i < index+1; i++)
        {
            next = next->pNext;
        }

        Node<T>* toDelete = previous->pNext; 
        previous->pNext = toDelete->pNext;  
        next->pPrev = toDelete->pPrev;
        delete toDelete; 

        size--;
    }
}


template <typename T>
void listContainer<T>::push_front(T data) // добавляет элемент в начало списка
{
    Head = new Node<T>(data, Head);
    size++;
}

template <typename T>
void listContainer<T>::insert(T data, int index) // добавляет элемент в начало списка
{
     if (index == 0)
    {
        push_front(data);
        return; // интересно так отработает, в 1С без проблем
    }
    else
    {
        Node<T>* previous = this->Head;  
        for (int i = 0; i < index - 1; i++)
        {
            previous = previous->pNext; 
        }
        Node<T>* newNode = new Node<T>(data, previous->pNext);
        previous->pNext = newNode;
        // Данный код можно сократить до одной строчки, будет выглядеть так:
        //previous -> pNext = new Node<T>(data, previous -> pNext);

        Node<T>* next = this->Head;
        for (int i = 0; i < index + 1; i++)
        {
            next = next->pNext;
        }
        next->pPrev = newNode;

        size++;
    }
}

