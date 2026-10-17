#include <iostream>
#include <vector>

using namespace std;

// Structure for a Doubly Threaded Binary Tree Node
struct Node {
    int data;
    Node *left, *right;
    bool lthread; // true if left points to predecessor, false if it points to left child
    bool rthread; // true if right points to successor, false if it points to right child

    Node(int val) : data(val), left(nullptr), right(nullptr), lthread(true), rthread(true) {}
};

// Function to insert a node in a Doubly Threaded BST
Node* insert(Node* root, int key) {
    Node* ptr = root;
    Node* parent = nullptr;

    // Search for the insertion point using threads to detect boundaries
    while (ptr != nullptr) {
        if (key == ptr->data) {
            cout << "Duplicate key " << key << " ignored.\n";
            return root;
        }
        parent = ptr;
        if (key < ptr->data) {
            if (ptr->lthread == false)
                ptr = ptr->left;
            else
                break;
        } else {
            if (ptr->rthread == false)
                ptr = ptr->right;
            else
                break;
        }
    }

    Node* tmp = new Node(key);

    if (parent == nullptr) {
        root = tmp;
    } else if (key < parent->data) {
        tmp->left = parent->left;
        tmp->right = parent;
        parent->lthread = false;
        parent->left = tmp;
    } else {
        tmp->left = parent;
        tmp->right = parent->right;
        parent->rthread = false;
        parent->right = tmp;
    }
    return root;
}

// Helper to find the leftmost node in a subtree
Node* leftmost(Node* n) {
    if (n == nullptr) return nullptr;
    while (n->lthread == false)
        n = n->left;
    return n;
}

// Helper to find the rightmost node in a subtree
Node* rightmost(Node* n) {
    if (n == nullptr) return nullptr;
    while (n->rthread == false)
        n = n->right;
    return n;
}

// 1. Display Predecessor and Successor threads for every node
void displayThreads(Node* root) {
    if (root == nullptr) return;
    
    // Inorder traversal to print threads sequentially
    if (root->lthread == false) displayThreads(root->left);
    
    cout << "Node [" << root->data << "]: ";
    if (root->lthread && root->left != nullptr)
        cout << "<- (Thread to Predecessor: " << root->left->data << ") ";
    else
        cout << "<- (No Left Thread) ";
        
    if (root->rthread && root->right != nullptr)
        cout << "-> (Thread to Successor: " << root->right->data << ") ";
    else
        cout << "-> (No Right Thread) ";
    cout << "\n";
    
    if (root->rthread == false) displayThreads(root->right);
}

// 2. Standard Inorder Traversal using threads (No recursion, No stack)
void threadedInorder(Node* root) {
    Node* cur = leftmost(root);
    while (cur != nullptr) {
        cout << cur->data << " ";
        if (cur->rthread)
            cur = cur->right; // Follow the thread directly to successor
        else
            cur = leftmost(cur->right); // Go to the leftmost node of the right child
    }
    cout << "\n";
}

// 3. Reverse Inorder Traversal using threads (No recursion, No stack)
void threadedReverseInorder(Node* root) {
    Node* cur = rightmost(root);
    while (cur != nullptr) {
        cout << cur->data << " ";
        if (cur->lthread)
            cur = cur->left; // Follow the thread directly to predecessor
        else
            cur = rightmost(cur->left); // Go to the rightmost node of the left child
    }
    cout << "\n";
}

// 4. Conventional BST Inorder Traversal (Recursive, ignoring threads)
void conventionalInorder(Node* root) {
    if (root == nullptr) return;
    if (root->lthread == false) conventionalInorder(root->left);
    cout << root->data << " ";
    if (root->rthread == false) conventionalInorder(root->right);
}

int main() {
    Node* root = nullptr;
    
    // Sample insertion sequence to build the tree
    vector<int> keys = {20, 10, 30, 5, 15, 25, 35};
    
    cout << "Inserting elements into Doubly Threaded BST: ";
    for (int key : keys) {
        cout << key << " ";
        root = insert(root, key);
    }
    cout << "\n\n";

    // Modification 1: Display threads
    cout << "=== 1. Node Threads Status ===\n";
    displayThreads(root);
    cout << "\n";

    // Modification 3: Compare Threaded Traversal vs Conventional Traversal
    cout << "=== 2. Traversal Comparison ===\n";
    cout << "Conventional Inorder (Recursive): ";
    conventionalInorder(root);
    cout << "\n";

    cout << "Threaded Inorder (Iterative):    ";
    threadedInorder(root);
    cout << "\n";

    // Modification 2: Reverse Inorder Traversal using threads
    cout << "=== 3. Reverse Threaded Traversal ===\n";
    cout << "Threaded Reverse Inorder:        ";
    threadedReverseInorder(root);
    cout << "\n";

    return 0;
}
