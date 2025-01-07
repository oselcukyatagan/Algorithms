
#include <iostream>
using namespace std;

struct Node
{
    /* data */
    int value;
    Node* link;

    Node(int val) : value(val), link(nullptr){};
};

void addNodeToEnd(Node* headNodePointer, int newNodeValue){

    Node* N = new Node(newNodeValue);
    
    Node* iterationLink = headNodePointer;
    while(iterationLink->link != nullptr){ 
        iterationLink = iterationLink -> link; // loop until the element youre pointing to is pointing to null. (final pointer is pointing to last element)
    }
    iterationLink->link = N;

    cout << "Added new node with the value " << newNodeValue << " at the end!" << endl; // iterationLink->link->value is the same as newNodeValue

}

void showLinkedList(Node* headNodePointer){

    cout << "The current state of the linked list: " << endl;

    Node* iterationLink = headNodePointer;
    while(iterationLink != nullptr){ // loop until the element youre pointing to is null. (final pointer is pointing to nothing.)
        cout << iterationLink->value << endl;
        iterationLink = iterationLink -> link;
    }

}

void deleteLinkedList(Node* &headpointer){  // passing headpointer by reference to free headpointer
    
    Node* iterationLink = headpointer;
    while(iterationLink != nullptr) {
        Node* temp = iterationLink;
        iterationLink = iterationLink->link;
        delete temp;
    }
    headpointer = nullptr;
}


int main(){

    Node* A = new Node(2);

    showLinkedList(A);

    for (uint16_t i = 0; i < 5; i++)
    {
        /* code */
        addNodeToEnd(A,i);
    }

    showLinkedList(A);

    deleteLinkedList(A);   

    return 0;
}