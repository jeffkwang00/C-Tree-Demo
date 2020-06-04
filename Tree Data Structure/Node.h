#ifndef __NODE_H
#define __NODE_H

#include <iostream>
#include <string>

using namespace std;

class Node {

  friend class Tree;

private:
  string small;
  string large;

  Node *left;
  Node *middle;
  Node *right;
  Node *parent;

  // Add additional functions/variables here. Remember, you may not add any
  // Node * or string variables.
    
public: //Remove on completion
    Node();
    
    void SetSmall (string);
    void SetLarge (string);
    void SetLeft (Node*);
    void SetMiddle (Node*);
    void SetRight (Node*);
    void SetParent (Node*);
    
    string GetSmall() const {return small;}
    string GetLarge() const {return large;}
    Node* GetLeft() const {return left;}
    Node* GetRight() const {return right;}
    Node* GetMiddle() const {return middle;}
    Node* GetParent() const {return parent;}
    
    
    bool isLeaf() const;
    void swap(); //swaps small and large string
    void PrintNode() const; //prints all variable values
    
    

};

#endif