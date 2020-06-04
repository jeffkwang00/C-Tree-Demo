#ifndef __TREE_H
#define __TREE_H

#include "Node.h"

class Tree {
private:
  Node *root;
  
  void inOrder(Node*) const;
  void preOrder(Node*) const;
  void postOrder(Node*) const;
  void PrintNode(Node*) const;  
    
public:
    
  //public for now
    inline Node* GetRoot() const {return root;}
    bool isEmpty() const {return root == NULL;}
    void split(Node*);
    bool isFull(Node*) const;
    bool sizeOne(Node*) const;
    void SetRoot(Node*);
    void Internalsplit(Node*, Node*);
    void CRootsplit(Node*, Node*);
    void Msplit(string, string, string, Node*);
    void Rsplit(string, string, string, Node*);
    void Lsplit(string, string, string, Node*);
    void Rootsplit(string, string, string);
    void split(Node*, string);
    Node* search(const string&, Node*) const;    
    void swapInOrder(Node*, Node*, const string &);
    void fix(Node*);
    Node* FullSibling(Node*, Node*);
    void redist(Node*, Node*);
    void merge(Node*, Node*);
    
  Tree( );
//   ~Tree( );
  void insert(const string &);
    
    /*
        if empty, insert into small
        else if not full
            if <= small value, insert into large and swap
            else, insert into large
        else
            create temp node with three values
            split                        
            connect split node with parent and parent with split node
    */
    
    
  void preOrder( ) const;
  void inOrder( ) const;
  void postOrder( ) const;
  void remove(const string &);
  bool search (const string &) const;

private:
  // Add additional functions/variables here
    
};

#endif