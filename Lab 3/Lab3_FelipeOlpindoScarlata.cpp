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
        if(head == NULL)                            // check if head(first node) exists
        {
            return NULL;                            // return NULL if it doesn't exist
        }
        else
        {
            return head;                            // get/return the head node if it exists
        }
    }

    // 2d. Getting the node after any other node
    IntNode* getNext(IntNode* node) {
        if(node->next == NULL)                      // check if the next node of a given node exists
        {
            return NULL;                            // return NULL if it doesn't exist
        }
        else
        {
            return node->next;                      // get/return the next node if it exists
        }
    }

    // 2e. Deleting a node (Assumes node is not Head)
    void deleteNode(IntNode* nodeToDelete) {
        IntNode* current = head;                                    // Start a temporary pointer at the beginning of the list
        while (current != NULL && current->next != nodeToDelete){   // Loop until we find the node situated *before* the target
            current = current->next;                                // Move the pointer to the next node
        }
        if (current != NULL){                                       // Check if we successfully found the node before the target
            current->next = nodeToDelete->next;                     // Link the previous node to the node *after* the target (bypass target)
            delete nodeToDelete;                                    // Free the memory of the target node
        }
    }

    // Destructor
    ~IntList() {
        IntNode* current = head;                      // Start a temporary pointer at the beginning
        while (current != NULL) {                     // Loop as long as there are nodes left in the list
            IntNode* temp = current;                  // Save the current node in a temporary variable
            current = current->next;                  // Move to the next node immediately so we don't lose the chain
            delete temp;                              // Delete the saved node to free memory
        }
    }
};


// 3. IntStack struct
// Implements a stack using IntNode internally
// IntNode is NOT exposed to the user
struct IntStack{
    private:
        IntNode* topNode;    // pointer to the top node of the stack (hidden from user)
        int stackSize;       // tracks the current size of the stack

    public:
        // Constructor initializes empty stack
        IntStack(){
            topNode = NULL;
            stackSize = 0;
        }

        // Push: add integer to the top of stack
        void push(int value){
            IntNode* newNode = new IntNode;     // Create a new node
            newNode->data = value;
            newNode->next = topNode;            // Link the new node to the current top
            topNode = newNode;                  // Update the top pointer
            stackSize++;                        // Increment size
        }

        // Pop: remove and return integer from top of stack
        int pop(){
            if (topNode == NULL) {       // Check if stack is empty                      
                cout << "Stack is empty. Cannot pop. Returning 0." << endl;
                return 0;
            }

            int value = topNode->data;   // Store the value to return
            IntNode* temp = topNode;     // Store the node to delete
            topNode = topNode->next;     // Move top pointer to next node
            delete temp;                 // Delete the old top node
            stackSize--;                 // Decrement size
            
            return value;
        }

        int size(){                     // Size: return current number of elements in stack
            return stackSize;
        }

        // Destructor
        ~IntStack(){
            while (topNode != NULL){         // Loop as long as the stack is not empty (top exists)
                IntNode* temp = topNode;     // Save the current top node into a temporary pointer
                topNode = topNode->next;     // Move the top pointer down to the next node in the stack
                delete temp;                 // Delete the old top node to free its memory
            }
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