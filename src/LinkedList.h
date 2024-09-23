//
// Created by Insung Seo on 2023/02/13.
//

#include <vector>
#include <iostream>

#ifndef COP3503C_LINKEDLIST_H
#define COP3503C_LINKEDLIST_H

using namespace std;

template <typename T>
class LinkedList {
public:
    class Node {
    public:
        T data;
        Node* next;
        Node* prev;

        Node();
        explicit Node(T data);
        Node(T data, Node* prev, Node* next);
        Node& operator=(const Node& node);
        ~Node();
    };

    // Behaviors
    void PrintForward() const;
    void PrintReverse() const;
    void PrintForwardRecursive(const Node* node) const;
    void PrintReverseRecursive(const Node* node) const;

    // Accessors
    [[nodiscard]] unsigned int NodeCount() const;
    void FindAll(std::vector<Node*>& outData, const T& value) const;
    const Node* Find(const T& data) const;
    Node* Find(const T& data);
    const Node* GetNode(unsigned int index) const;
    Node* GetNode(unsigned int index);
    const Node* Head() const;
    Node* Head();
    const Node* Tail() const;
    Node* Tail();

    // Insertion
    void AddHead(const T& data);
    void AddTail(const T& data);
    void AddNodesHead(const T* data, unsigned int count);
    void AddNodesTail(const T* data, unsigned int count);
    void InsertAfter(Node* node, const T& data);
    void InsertBefore(Node* node, const T& data);
    void InsertAt(const T& data, unsigned int count);

    // Removal
    bool RemoveHead();
    bool RemoveTail();
    unsigned int Remove(const T& data);
    bool RemoveAt(unsigned int index);
    T Pop(unsigned int index);
    void Clear();

    // Operators
    const T& operator[](unsigned int index) const;
    T& operator[](unsigned int index);
    bool operator==(const LinkedList<T>& rhs) const;
    LinkedList<T>& operator=(const LinkedList<T>& rhs);

    // Construction, Destruction
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();

private:
    unsigned int length;
    Node* HeadNode;
    Node* TailNode;
};


// ======= Nodes =======
template <typename T>
LinkedList<T>::Node::Node() {
    this->data = T();
    this->next = this->prev = nullptr;
}

template <typename T>
LinkedList<T>::Node::Node(T data) {
    this->data = data;
    this->next = this->prev = nullptr;
}

template <typename T>
LinkedList<T>::Node::Node(T data, Node* prev, Node* next) {
    this->data = data;
    this->prev = prev;
    this->next = next;
}

template <typename T>
typename LinkedList<T>::Node& LinkedList<T>::Node::operator=(const Node& node) {
    if (this != &node) {
        data = node.data;
        prev = node.prev;
        next = node.next;
    }
    return *this;
}

template <typename T>
LinkedList<T>::Node::~Node() = default;


// ======= Behaviors =======
template <typename T>
void LinkedList<T>::PrintForward() const {
    for (Node* current = HeadNode; current != nullptr; current = current->next) {
        cout << current->data << endl;
    }
}

template <typename T>
void LinkedList<T>::PrintReverse() const {
    for (Node* current = TailNode; current != nullptr; current = current->prev) {
        cout << current->data << endl;
    }
}

template <typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const {
    cout << node->data << endl;
    if (node->next != nullptr) {
        return PrintForwardRecursive(node->next);
    } else {
        return;
    }
}

template <typename T>
void LinkedList<T>::PrintReverseRecursive(const Node* node) const {
    cout << node->data << endl;
    if (node->prev != nullptr) {
        return PrintReverseRecursive(node->prev);
    } else {
        return;
    }
}

// ======= Accessors =======
template <typename T>
unsigned int LinkedList<T>::NodeCount() const {
    return length;
}

template <typename T>
void LinkedList<T>::FindAll(std::vector<Node*>& outData, const T& value) const {
    for (Node* current = HeadNode; current != nullptr; current = current->next) {
        if (current->data == value) {
            outData.push_back(current);
        }
    }
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const {
    for (Node* current = HeadNode; current != nullptr; current = current->next) {
        if (current->data == data) {
            return current;
        }
    }
    return nullptr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) {
    for (Node* current = HeadNode; current != nullptr; current = current->next) {
        if (current->data == data) {
            return current;
        }
    }
    return nullptr;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) const {
    if (index >= length) {
        throw out_of_range("");
    }

    Node* result = HeadNode;
    for (unsigned int i = 0; i < index; i++) {
        result = result->next;
    }

    return result;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) {
    if (index >= length) {
        throw out_of_range("");
    }

    Node* result = HeadNode;
    for (unsigned int i = 0; i < index; i++) {
        result = result->next;
    }

    return result;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const {
    return HeadNode;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
    return HeadNode;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const {
    return TailNode;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
    return TailNode;
}

// ======= Insertion =======
template <typename T>
void LinkedList<T>::AddHead(const T& data) {
    HeadNode = new Node(data, nullptr, HeadNode);

    if (!TailNode) {
        TailNode = HeadNode;
    }

    if (HeadNode->next) {
        (HeadNode->next)->prev = HeadNode;
    }

    length++;
}

template <typename T>
void LinkedList<T>::AddTail(const T& data) {
    TailNode = new Node(data, TailNode, nullptr);

    if (!HeadNode) {
        HeadNode = TailNode;
    }

    if (TailNode->prev) {
        (TailNode->prev)->next = TailNode;
    }

    length++;
}

template <typename T>
void LinkedList<T>::AddNodesHead(const T* data, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        this->AddHead(data[count - 1 - i]);
    }

    length += count;
}

template <typename T>
void LinkedList<T>::AddNodesTail(const T* data, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        this->AddTail(data[i]);
    }

    length += count;
}

template <typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data) {
    Node* newNode = new Node(data, node->prev, node);
    if (node->prev)
        node->prev->next = newNode;
    node->prev = newNode;
    length++;
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data) {
    Node* newNode = new Node(data, node, node->next);
    if (node->next)
        node->next->prev = newNode;
    node->next = newNode;
    length++;
}

template <typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int count) {
    if (count > length) {
        throw out_of_range("");
    }
    else if (count == 0) {
        this->AddHead(data);
    }
    else if (count == length) {
        this->AddTail(data);
    }
    else {
        Node* nodePtr = this->GetNode(count);
        Node* newNode = new Node(data, nodePtr->prev, nodePtr);
        newNode->prev->next = newNode;
        newNode->next->prev = newNode;
        length++;
    }
}

// Removal
template <typename T>
bool LinkedList<T>::RemoveHead() {
    if (!HeadNode)
        return false;

    if (!HeadNode->next){
        delete HeadNode;
        HeadNode = TailNode = nullptr;

    } else {
        HeadNode = HeadNode->next;
        delete HeadNode->prev;
        HeadNode->prev = nullptr;
    }

    length--;
    return true;
}

template <typename T>
bool LinkedList<T>::RemoveTail() {
    if (!TailNode)
        return false;

    if (!TailNode->prev){
        delete TailNode;
        HeadNode = TailNode = nullptr;

    } else {
        TailNode = TailNode->prev;
        delete TailNode->next;
        TailNode->next = nullptr;
    }

    length--;
    return true;
}

template <typename T>
unsigned int LinkedList<T>::Remove(const T& data) {
    vector<Node*> nodesRemove;
    this->FindAll(nodesRemove, data);
    unsigned int removedNum = nodesRemove.size();

    for (unsigned int i = 0; i < removedNum; i++) {
        Node* tempNodePtr = nodesRemove.at(i);

        if (!tempNodePtr->prev && !tempNodePtr->next) { // if removing the only node
            HeadNode = TailNode = nullptr;
        }
        else if (!tempNodePtr->prev) { // if removing the first node
            tempNodePtr->next->prev = nullptr;
            HeadNode = tempNodePtr->next;
        }
        else if (!tempNodePtr->next) { // if removing the last node
            tempNodePtr->prev->next = nullptr;
            TailNode = tempNodePtr->prev;
        }
        else {
            tempNodePtr->next->prev = tempNodePtr->prev;
            tempNodePtr->prev->next = tempNodePtr->next;
        }
        delete tempNodePtr;
    }

    length -= removedNum;
    return removedNum;
}

template <typename T>
bool LinkedList<T>::RemoveAt(unsigned int index) {
    if (index > length) {
        return false;
    }

    Node* tempNodePtr = this->GetNode(index);
    if (!tempNodePtr->prev && !tempNodePtr->next) { // if removing the only node
        HeadNode = TailNode = nullptr;
    }
    else if (!tempNodePtr->prev) { // if removing the first node
        tempNodePtr->next->prev = nullptr;
        HeadNode = tempNodePtr->next;
    }
    else if (!tempNodePtr->next) { // if removing the last node
        tempNodePtr->prev->next = nullptr;
        TailNode = tempNodePtr->prev;
    }
    else {
        tempNodePtr->next->prev = tempNodePtr->prev;
        tempNodePtr->prev->next = tempNodePtr->next;
    }

    length--;
    delete tempNodePtr;
    return true;
}

template <typename T>
T LinkedList<T>::Pop(unsigned int index) {
    if (index > length) {
        throw out_of_range("linkedList: out of range");
    }

    Node* tempNodePtr = this->GetNode(index);
    if (!tempNodePtr->prev && !tempNodePtr->next) { // if removing the only node
        HeadNode = TailNode = nullptr;
    }
    else if (!tempNodePtr->prev) { // if removing the first node
        tempNodePtr->next->prev = nullptr;
        HeadNode = tempNodePtr->next;
    }
    else if (!tempNodePtr->next) { // if removing the last node
        tempNodePtr->prev->next = nullptr;
        TailNode = tempNodePtr->prev;
    }
    else {
        tempNodePtr->next->prev = tempNodePtr->prev;
        tempNodePtr->prev->next = tempNodePtr->next;
    }

    length--;
    T result = tempNodePtr->data;
    delete tempNodePtr;
    return result;
}

template <typename T>
void LinkedList<T>::Clear() {
    if (HeadNode) {
        for (HeadNode = HeadNode->next; HeadNode != nullptr; HeadNode = HeadNode->next) {
            delete HeadNode->prev;
        }
        delete TailNode;
        length = 0;
        HeadNode = TailNode = nullptr;
    }
}

// Operators
template <typename T>
const T& LinkedList<T>::operator[](unsigned int index) const {
    if (index >= length) {
        throw out_of_range("");
    }

    Node* resultPtr = HeadNode;
    for (unsigned int i = 0; i < index; i++) {
        resultPtr = resultPtr->next;
    }

    return resultPtr->data;
}

template <typename T>
T& LinkedList<T>::operator[](unsigned int index) {
    if (index >= length) {
        throw out_of_range("");
    }

    Node* resultPtr = HeadNode;
    for (int i = 0; i < index; i++) {
        resultPtr = resultPtr->next;
    }

    return resultPtr->data;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) const {
    if (this->length != rhs.length)
        return false;

    Node* current = HeadNode;
    Node* rhsCurrent = rhs.HeadNode;
    for (; current != nullptr; current = current->next, rhsCurrent = rhsCurrent->next) {
        if (current->data != rhsCurrent->data)
            return false;
    }

    return true;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
    if (this != &rhs) {
        if (HeadNode) {
            for (HeadNode = HeadNode->next; HeadNode != nullptr; HeadNode = HeadNode->next) {
                delete HeadNode->prev;
            }
            delete TailNode;
        }

        this->length = rhs.length;
        Node* current = HeadNode = new Node(rhs.HeadNode->data);
        Node* rhsCurrent = rhs.HeadNode->next;
        for (; rhsCurrent != nullptr; current = current->next, rhsCurrent = rhsCurrent->next) {
            current->next = new Node(rhsCurrent->data, current, nullptr);
        }
        TailNode = current;
    }

    return *this;
}

// Construction, Destruction
template <typename T>
LinkedList<T>::LinkedList() {
    this->HeadNode = this->TailNode = nullptr;
    length = 0;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
    length = list.length;
    Node* current = HeadNode = new Node(list.HeadNode->data);
    Node* rhsCurrent = list.HeadNode->next;
    for (; rhsCurrent != nullptr; current = current->next, rhsCurrent = rhsCurrent->next) {
        current->next = new Node(rhsCurrent->data, current, nullptr);
    }
    TailNode = current;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    if (HeadNode) {
        for (HeadNode = HeadNode->next; HeadNode != nullptr; HeadNode = HeadNode->next) {
            delete HeadNode->prev;
        }
        delete TailNode;
    }
}

#endif //COP3503C_LINKEDLIST_H
