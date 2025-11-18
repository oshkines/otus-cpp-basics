#pragma once

template<typename T>
class listContainer
{
private:
    int size;

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
    Node* Head;
public:
    listContainer(int size);
    ~listContainer();
    void push_back(T value);
    void clear();
    void pop_front();
    int getSize() { return size; }
    bool emptry(){ return size == 0; }   
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

    Node* Temp = Head; 
    Head = Head->pNext;
    if (Head != nullptr) { Head->pPrev = nullptr; }
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
        Head = new Node(data, nullptr, nullptr);
    }
    else
    {
        Node* Current = this->Head; 
        while (Current->pNext != nullptr)
        {
            Current = Current->pNext;
        }
        Current->pNext = new Node(data, nullptr, Current);
    }

     size++;

}

template <typename T>
T& listContainer<T>::operator[](const int index)
{
    int counter = 0; 
    Node* Current = Head; 

    while (Current != nullptr) {
        if (counter == index)
        {
            return Current->data;
        }

        Current = Current->pNext;
        counter++;
    }
    return Head->data;
}

template <typename T>
void listContainer<T>::removeAt(const int index)
{
    int counter = 0;
    Node* toDelete = Head;

    while (toDelete != nullptr) {
        if (counter == index)
        {
            break;
        }
        toDelete = toDelete->pNext;
        counter++;
    }



    if (toDelete->pPrev == nullptr)//(index == 0)
    {
        pop_front();
    }
    else
    {
        //Node* previous = this->Head;
        //for (int i = 0; i < index - 1; i++)
        //{
        //    previous = previous->pNext; 
        //}

        //Node* next = this->Head;
        //for (int i = 0; i < index+1; i++)
        //{
        //    next = next->pNext;
        //}

        Node* previous = toDelete->pPrev;
        Node* next = toDelete ->pNext;

        //Node* toDelete = previous->pNext; 
        if (previous != nullptr){ previous->pNext = toDelete->pNext; }
        if (next != nullptr) { next->pPrev = toDelete->pPrev; }

        delete toDelete; 

        size--;
    }
}


template <typename T>
void listContainer<T>::push_front(T data) // добавляет элемент в начало списка
{
    Head = new Node(data, Head);
    size++;
}

template <typename T>
void listContainer<T>::insert(T data, int index) // добавляет элемент в начало списка
{
     if (index == 0)
    {
        Node* next = this->Head;
        push_front(data);
        if (size > 1) { next->pPrev = Head; }
        return; // интересно так отработает, в 1С без проблем
    }
    else
    {
         //this[0]
        Node* previous = this->Head;  
        for (int i = 0; i < index - 1; ++i)
        {
            previous = previous->pNext; 
        }

        // Данный код можно сократить до одной строчки, будет выглядеть так:
        //previous -> pNext = new Node<T>(data, previous -> pNext);
        Node* next = this->Head;
        if (size > index) {
            //Node* next = this->Head;
            for (int i = 0; i < index; i++)
            {
                next = next->pNext;
            }

        }

        Node* newNode = new Node(data, next, previous);//new Node(data, previous->pNext);
        previous->pNext = newNode;
        next->pPrev = newNode;
        //newNode->pNext = next;

        size++;
    }
}
