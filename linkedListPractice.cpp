
#include <iostream>

struct Node
{
    int value;
    Node* link;  

    Node(int val) : value(val), link(nullptr){};
};

void Insert_Node_At_Beginning(Node* &Head_Node, int Node_Value){
    Node* N = new Node(Node_Value);
    N->link = Head_Node;
    N = Head_Node;
}

void Display_List(Node* Head_Node){
    while ( Head_Node != nullptr )
    {
        /* code */
        std::cout << Head_Node->value << std::endl;
        Head_Node = Head_Node->link;
    }
    
}

int main(){

    
    Node* Head_Node = new Node(2);

    Insert_Node_At_Beginning(Head_Node, 2);
    Display_List(Head_Node);

    Insert_Node_At_Beginning(Head_Node, 3);
    Display_List(Head_Node);
    

    Node* i = Head_Node;
    while (i != nullptr)
    {
        Node* temp = i;
        i = i->link;
        delete temp;
    }
    

    return 0;
}