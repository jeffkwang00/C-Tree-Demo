#include "Node.h"

Node::Node() : small(""), large(""), left(0), middle(0), right(0), parent(0) {}

void Node::SetSmall (string str) {small = str;} 
void Node::SetLarge (string str) {large = str;}
void Node::SetLeft (Node* n) {left = n;}
void Node::SetMiddle (Node* n) {middle = n;}
void Node::SetRight (Node* n) {right = n;}
void Node::SetParent (Node* n) {parent = n;}

void Node::swap() {    
    string temp = small;
    
    small = large;
    large = temp;
}

void Node::PrintNode() const {
    cout << small << "|" << large;
}

bool Node::isLeaf() const {
    return (((left == NULL) && (middle == NULL)) && (right == NULL));
}