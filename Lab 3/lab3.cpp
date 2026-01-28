#include <iostream>
using namespace std;

// 1. IntNode struct
// Represents a single node in a linked structure
struct IntNode {
    int data;        // integer stored in the node
    IntNode* next;   // pointer to the next node
};

// 2. IntList struct
// Manages a linked list of IntNode objects
struct IntList {
    IntNode* head;   // pointer to the first node in the list

    // Constructor initializes the list as empty
    IntList() {
        head = NULL;
    }

    // 2a. Create the first node of the list
    IntNode* createFirst(int value) {
        // If a first node already exists, return it
        if (head != NULL)
            return head;

        // Dynamically allocate memory for the first node
        head = new IntNode;
        head->data = value;
        head->next = NULL;

        return head;
    }

    // 2b. Insert a node after a given node
    IntNode* insertAfter(IntNode* node, int value) {
        // Safety check
        if (node == NULL)
            return NULL;

        // Allocate memory for the new node
        IntNode* newNode = new IntNode;
        newNode->data = value;

        // Link the new node into the list
        newNode->next = node->next;
        node->next = newNode;

        return newNode;
    }

    // 2c. Getting the first node
    IntNode* getFirst() {
        if(head == NULL)
        {
            return NULL;
        }
        else
        {
            return head;
        }
    }

    // 2d. Getting the node after any other node
    IntNode* getNext(IntNode* node) {
        if(node->next == NULL)
        {
            return NULL;
        }
        else
        {
            return node->next;
        }
    }

    // 2e. Deleting a node
    deleteNode(IntNode* node) {
        delete node;
        node = NULL;
    }
};

//temporary main
int main() {
    IntList list;

    IntNode* first = list.createFirst(6);
    list.insertAfter(first, 67);
    list.insertAfter(first, 7);

    // traverse the list
    IntNode* current = list.head;
    while (current != NULL) {
        cout << current->data << " ";
        current = current->next;
    }

    cout << endl;

    return 0;
}