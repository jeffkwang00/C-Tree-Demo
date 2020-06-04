#include "Tree.h"

void Tree::SetRoot(Node* n) {root = n;}

Tree::Tree() : root(0) {}

/*<
Tree::~Tree() {
}
*/

bool Tree::isFull(Node* n) const {return ((n -> GetSmall() != "") && (n -> GetLarge() != ""));}
bool Tree::sizeOne(Node* n) const {return n -> GetLarge() == "";}

void Tree::insert(const string& str) {
    
    /* 
        If empty, create new node at root.
        else,
            iterate to correct leaf node
            if node still has space, insert
            else split            
        
    */
    
    if (isEmpty()) {
        root = new Node();
        root -> SetSmall(str);
    } 
    
    else {
        
        /*
        
        1. Locate leaf
        2. If leaf is only size one
        3. If leaf is full
            a. "Add value to leaf"
            b. Determine small, median, large
            c. 
        
        */
        
        //STEP ONE
        Node* currNode = root;
        
        while (!(currNode -> isLeaf())) {
            
            if (str <= currNode -> GetSmall())
                
                currNode = currNode -> GetLeft();
            
            else if ((str < currNode -> GetLarge()) && (isFull(currNode))) {
            
                currNode = currNode -> GetMiddle();
                
            } else {
                currNode = currNode -> GetRight();
            }
        }
        
        //STEP TWO
        if ((sizeOne(currNode)) && (!isFull(currNode))) {

            currNode -> SetLarge(str);

            if (str < currNode -> GetSmall())
                currNode -> swap();

        }
        
        
        else {
            split(currNode, str);
            
        }
          
    
    } //else close    
} //function close

void Tree::split(Node* splitNode, string newKey) {    
    Node* parent = splitNode -> GetParent();
    
    string sm = splitNode -> GetSmall();
    string med = "";
    string lg = splitNode -> GetLarge();
    
    //Determine order (S, M, L)
        if (newKey <= sm) {
            sm = newKey;
            med = splitNode -> GetSmall();
            lg = splitNode -> GetLarge();
        }
        else if ((newKey <= lg) && (newKey > sm)) {
            med = newKey;
        } else {
            med = lg;
            lg = newKey;
        }
        
    if (splitNode == root) {
        Rootsplit(sm, med, lg);
    }
    //Split left node
    else if (parent -> GetLeft() == splitNode) {
        Lsplit(sm, med, lg, parent);
    }
    //Split right node
    else if (parent -> GetRight() == splitNode) {
        Rsplit(sm, med, lg, parent);
    }
    //Split middle node
    else if (parent -> GetMiddle() == splitNode) {
        Msplit(sm, med, lg, parent);
    }
    else
        cout << "No match..." << endl;
        
}

void Tree::Msplit (string sm, string med, string lg, Node* parent) { //parent node must be full
    Node* smNode = new Node();
        smNode -> SetSmall(sm);
    Node* lgNode = new Node();
        lgNode -> SetSmall(lg);
    Node* medNode = new Node();
        medNode -> SetSmall(med);
        medNode -> SetLeft(smNode);
        medNode -> SetRight(lgNode);
        medNode -> SetParent(parent); //changed from parent->GetParent() to parent
        smNode -> SetParent(medNode);
        lgNode -> SetParent(medNode);
    //parent's middle node is not set to medNode
    
    
    Internalsplit(parent, medNode);
}

void Tree::Internalsplit (Node* splitNode, Node* newNode) {
    Node* parent = splitNode -> GetParent();
    string key = newNode -> GetSmall();
    
    if (splitNode == root) {
        if (isFull(root)) //parent is root and full
            CRootsplit(root, newNode);
        else {
            
            cout << "final checkpoint." << endl;
            
            //insert in correct, empty position
            Node* left = newNode -> GetLeft();
            Node* right = newNode -> GetRight();
            
            splitNode -> SetLarge(key);
            
            if (key < splitNode -> GetSmall()) {
                splitNode -> swap();
                
                splitNode -> SetLeft(left);
                splitNode -> SetMiddle(right);                
            }
            else {
                splitNode -> SetMiddle(left);
                splitNode -> SetRight(right);                
            }
            
            left -> SetParent(splitNode);
            right -> SetParent(splitNode);
                
            delete newNode;
                        
        }
    }
    else {

        if (isFull(splitNode)) { //parent is full

            /*
            1) Check if medNode is left, middle, or right child
            2) in the case of a middle child, medNode becomes the parent of left and right of prev parent, and
                the prev left parent takes the two leftmost nodes while the prev right parent takes the two rightmost
            3) Internal split until root
            */
            
            
            
            if (key < splitNode -> GetSmall()) {
                //new med node is splitNode -> GetSmall()
                Node* middle = splitNode -> GetMiddle();
                Node* right = splitNode -> GetRight();
                
                Node* medNode = new Node();
                    medNode -> SetSmall(splitNode -> GetSmall());
                Node* lgNode = new Node();
                    lgNode -> SetSmall(splitNode -> GetLarge());
                    lgNode -> SetLeft(middle);
                    lgNode -> SetRight(right);
                    lgNode -> SetParent(medNode);
                
                medNode -> SetLeft(newNode);
                medNode -> SetRight(lgNode);
                
                newNode -> SetParent(medNode);
                middle -> SetParent(lgNode);
                right -> SetParent(lgNode);
                medNode -> SetParent(parent -> GetParent());
                
                delete splitNode;
                Internalsplit(parent, medNode);
            
            
            }
            else if ((key > splitNode -> GetSmall()) && (key < splitNode -> GetLarge())) {
                Node* left = newNode->GetLeft();
                Node* right = newNode->GetRight();
                Node* leftSplit = splitNode->GetLeft();
                Node* rightSplit = splitNode->GetRight();

                cout << "checkpoint one." << endl;
                
                //no change; med node remains median
                //don't need to change parent; see Msplit
                Node* smNode = new Node();
                    smNode -> SetSmall(splitNode -> GetSmall());
                    smNode -> SetLeft(leftSplit);
                    smNode -> SetRight(left);
                
                Node* lgNode = new Node();
                    lgNode -> SetSmall(splitNode -> GetLarge());
                    lgNode -> SetLeft(right);
                    lgNode -> SetRight(rightSplit);
                
                Node* medNode = newNode;
                    medNode -> SetLeft(smNode);
                    medNode -> SetRight(lgNode);
                    medNode -> SetParent(parent -> GetParent()); //for tree height of 2, node's parent is already nullptr
                
                smNode -> SetParent(medNode);
                lgNode -> SetParent(medNode);
                left->SetParent(smNode);
                right->SetParent(lgNode);
                leftSplit->SetParent(smNode);
                rightSplit->SetParent(lgNode);
                
                delete splitNode; //delete split node to avoid memory leak
                Internalsplit(parent, medNode);
            }
            else {
                //new med node is splitNode -> GetLarge()
                Node* middle = splitNode -> GetMiddle();
                Node* left = splitNode -> GetLeft();
                
                Node* smNode = new Node();
                    smNode -> SetSmall(splitNode -> GetSmall());
                    smNode -> SetLeft(left);
                    smNode -> SetRight(middle);
                
                Node* medNode = new Node();
                    medNode -> SetSmall(splitNode -> GetLarge());
                    medNode -> SetParent(parent -> GetParent());
                
                medNode -> SetLeft(smNode);
                medNode -> SetRight(newNode);
                
                smNode -> SetParent(medNode);
                newNode -> SetParent(medNode);
                
                middle -> SetParent(smNode);
                left -> SetParent(smNode);
                
                delete splitNode;
                Internalsplit(parent, medNode);
            
            }

        }
    }   
    
}     

void Tree::Lsplit (string sm, string med, string lg, Node* parent) {    
    //Creating new nodes
    Node* smNode = new Node();
    smNode -> SetSmall(sm);
    Node* lgNode = new Node();
    lgNode -> SetSmall(lg);

    if (isFull(parent)) { //Internal node split
        Node* medNode = new Node();
            medNode -> SetSmall(med);
            medNode -> SetLeft(smNode);
            medNode -> SetRight(lgNode);
            medNode -> SetParent(parent -> GetParent()); //CHECK
            smNode -> SetParent(medNode);
            lgNode -> SetParent(medNode);
        
        Internalsplit(parent, medNode);
    } 
    else {
        smNode -> SetParent(parent);
        lgNode -> SetParent(parent);
        
        parent -> SetLeft(smNode);
        parent -> SetMiddle(lgNode);
        
        parent -> SetLarge(med);

        if (med < parent -> GetSmall())
            parent -> swap();
    }
} 

void Tree::CRootsplit (Node* splitNode, Node* newNode) { //Root split with children
    string newKey = newNode -> GetSmall();
    
    if (newKey < splitNode -> GetSmall()) {
        Node* middle = splitNode -> GetMiddle();
        Node* right = splitNode -> GetRight();
        
        Node* medNode = new Node();
            medNode -> SetSmall(splitNode -> GetSmall());
        Node* lgNode = new Node();
            lgNode -> SetSmall(splitNode -> GetLarge());
            lgNode -> SetLeft(middle); //Set large node to rightmost nodes
            lgNode -> SetRight(right);
        
        medNode -> SetLeft(newNode);
        medNode -> SetRight(lgNode);
        
        newNode -> SetParent(medNode);
        lgNode -> SetParent(medNode);
        
        middle -> SetParent(lgNode);
        
        delete root;
        root = medNode;
    }
    else if (newKey < splitNode -> GetLarge()) {
        Node* left = newNode -> GetLeft();
        Node* right = newNode -> GetRight();
        Node* smNode = new Node();
            smNode -> SetSmall(splitNode -> GetSmall());
            smNode -> SetLeft(splitNode -> GetLeft()); //Set small node to leftmost nodes
            splitNode -> GetLeft() -> SetParent(smNode);
            smNode -> SetRight(left);
        Node* lgNode = new Node();
            lgNode -> SetSmall(splitNode -> GetLarge());
            lgNode -> SetLeft(right); //Set large node to rightmost nodes
            lgNode -> SetRight(splitNode -> GetRight());
            splitNode -> GetRight() -> SetParent(lgNode);
        
        newNode -> SetLeft(smNode);
        newNode -> SetRight(lgNode);
        
        smNode -> SetParent(newNode);
        lgNode -> SetParent(newNode);
        
        left -> SetParent(smNode);
        right -> SetParent(lgNode);
        
        delete root;
        root = newNode;
    }
    else {
        Node* left = splitNode -> GetLeft();
        Node* middle = splitNode -> GetMiddle();
        
        Node* smNode = new Node();
            smNode -> SetSmall(splitNode -> GetSmall());
            smNode -> SetLeft(left); //Set small node to leftmost nodes
            smNode -> SetRight(middle);
        Node* medNode = new Node();
            medNode -> SetSmall(splitNode -> GetLarge());
        
        medNode -> SetLeft(smNode);
        medNode -> SetRight(newNode);
        
        newNode -> SetParent(medNode);
        smNode -> SetParent(medNode);
        
        middle -> SetParent(smNode); //middle was originally connected to splitted root
        left->SetParent(smNode);

        delete root;
        root = medNode;
    }    
}

void Tree::Rootsplit (string sm, string med, string lg) {
    Node* medNode = new Node();
        medNode -> SetSmall(med);
    Node* smNode = new Node();
        smNode -> SetSmall(sm);
    Node* lgNode = new Node();
        lgNode -> SetSmall(lg);
    
    medNode -> SetLeft(smNode);
    medNode -> SetRight(lgNode);
    smNode -> SetParent(medNode);
    lgNode -> SetParent(medNode);
    
    delete root;
    root = medNode;
}

void Tree::Rsplit(string sm, string med, string lg, Node* parent) {
    
    //Creating new nodes
    Node* smNode = new Node();
    smNode -> SetSmall(sm);
    Node* lgNode = new Node();
    lgNode -> SetSmall(lg);

    if (isFull(parent)) {
        Node* medNode = new Node();
            medNode -> SetSmall(med);
            medNode -> SetLeft(smNode);
            medNode -> SetRight(lgNode);
            medNode -> SetParent(parent -> GetParent());
            smNode -> SetParent(medNode);
            lgNode -> SetParent(medNode);
        
        Internalsplit(parent, medNode);
    }
    else {
        if (parent == NULL) {
            cout << "NULL parent warning : " << med << endl;
        }
        
        smNode -> SetParent(parent);
        lgNode -> SetParent(parent);
        
        parent -> SetMiddle(smNode);
        parent -> SetRight(lgNode);
        
        parent -> SetLarge(med);    
    }
}

bool Tree::search(const string &str) const {
    
    if (search(str, root))
        return true;
    
    return false;
}

Node* Tree::search(const string &str, Node* curr) const {
    
    if (curr) {
        
        if ((str == curr->GetSmall()) || (str == curr->GetLarge())) {
            return curr;
        }
        
        if (str < curr -> GetSmall())
            return search(str, curr -> GetLeft());
        else if ((str > curr -> GetSmall()) && (str < curr -> GetLarge()))
            return search(str, curr -> GetMiddle());
        else
            return search(str, curr -> GetRight());   
    
        
    }
    
    return NULL;
}

void Tree::remove(const string &str) {
    Node* removeLoc = search(str, root);
    
    if (removeLoc) {
        
        Node* leaf = nullptr;
        
        if (!removeLoc -> isLeaf()) {
            
            //swap removeLoc value with inorder successor (presumably a leaf)
            if (str == removeLoc -> GetSmall()) {
                swapInOrder(removeLoc, removeLoc -> GetLeft(), str);
                leaf = removeLoc -> GetLeft();
            } else {
                swapInOrder(removeLoc, removeLoc -> GetRight(), str);
                leaf = removeLoc -> GetRight();
            }
            
        } else {
            
            leaf = removeLoc;
            
        }
        
        if (leaf -> GetSmall() == str) {
            leaf -> SetSmall("");
            leaf -> swap();
        }
        else
            leaf -> SetLarge("");
        
        if ((leaf -> GetSmall() == "") && (leaf -> GetLarge() == ""))
            fix(leaf);
        

    }
}


void Tree::fix(Node* loc) {
    
    if (loc == root) {
        if (loc -> GetLeft() != NULL)
            root = loc -> GetLeft();
        else if (loc -> GetRight() != NULL)
            root = loc -> GetRight();
        else
            root = nullptr;
        
        delete loc;
    } else {
        Node* parent = loc -> GetParent();
        Node* sibling = FullSibling(parent, loc);
        
        if (sibling) {
            redist(loc, sibling);
            
            if (!loc -> isLeaf()) {
                //ADJUST CHILDREN
                
                if (loc == parent -> GetLeft()) {
                    loc -> SetLeft(loc -> GetRight());
                    loc -> SetRight(sibling -> GetLeft());
                    
                    sibling -> SetLeft(sibling -> GetMiddle());
                    sibling -> SetMiddle(nullptr);
                }
            }
        } else {
            
            if (sizeOne(parent)) {
                if (parent -> GetLeft() == loc)
                    sibling = parent -> GetRight();
                else
                    sibling = parent -> GetLeft();
            } else {
                if (parent -> GetMiddle() == loc) {
                    sibling = parent -> GetLeft(); 
                } else
                    sibling = parent -> GetMiddle();
            }
            
            merge(loc, sibling);
        }
        
    }
}

Node* Tree::FullSibling(Node* parent, Node* loc) {
    Node* sibling = nullptr;

    if (parent->GetLeft() == loc) {
        
        if ((isFull(parent)) && (isFull(parent->GetMiddle()))) //middle always first
            sibling = parent->GetMiddle();
        else if (isFull(parent->GetRight()))
            sibling = parent->GetRight();
    
    }
    else if (parent->GetMiddle() == loc) {
    
        if (isFull(parent->GetLeft()))
            sibling = parent->GetLeft();
        else if (isFull(parent -> GetRight()))
            sibling = parent->GetRight();
    
    }
    else if (parent->GetRight() == loc) {
    
        if (isFull(parent) && isFull(parent->GetMiddle()))
            sibling = parent->GetMiddle();
        else if (isFull(parent->GetLeft()))
            sibling = parent->GetLeft();
    
    }

    return sibling;
}

void Tree::redist(Node* loc, Node* fullSibling) {
    //loc is the empty leaf node and sibling is the adjacent node with 2+ values

    Node* parent = loc->GetParent();

    if (loc == fullSibling)
        return;
    
    if (parent->GetLeft() == loc) { //left case, counterclockwise movement
             
        loc->SetSmall(parent->GetSmall());
        
        if ((sizeOne(parent)) && (parent -> GetRight() == fullSibling)) {
            Node* right = parent -> GetRight();
            
            parent->SetSmall(right -> GetSmall());
            
            right -> swap();
            
            right -> SetLarge("");
        } else if (parent -> GetMiddle()) {
            
            if (fullSibling == parent -> GetMiddle()) { //either full sibling or not full sibling
                parent -> SetSmall(parent -> GetMiddle() -> GetSmall());
                parent -> GetMiddle() -> swap();
                parent -> GetMiddle() -> SetLarge("");
            }
            else
                parent -> SetSmall(parent -> GetMiddle() -> GetSmall());
            
            redist(parent -> GetMiddle(), fullSibling);
        } else {
            cout << "In redist : left error." << endl;
        }
        
    }
    else if (parent->GetMiddle() == loc) {
            
        if (parent->GetLeft() == fullSibling) {
            loc -> SetSmall(parent -> GetSmall());
            
            parent -> SetSmall(fullSibling -> GetLarge());
            
            fullSibling -> SetLarge("");
        }
        else {            
            loc -> SetSmall(parent -> GetLarge());
            
            parent -> SetLarge(fullSibling -> GetSmall());
            
            fullSibling -> swap();
            fullSibling -> SetLarge("");
        }
    }
    else {
        
        loc->SetSmall(parent->GetLarge());
        
        if ((sizeOne(parent)) && (parent -> GetLeft() == fullSibling)) {
            Node* left = parent -> GetLeft();
            
            loc -> SetSmall(parent -> GetSmall());
            
            parent->SetSmall(left -> GetLarge());
            
            left -> SetLarge("");
        } else if (parent -> GetMiddle()) {
            
            if (fullSibling == parent -> GetMiddle()) { //either full sibling or not full sibling
                
                parent -> SetLarge(parent -> GetMiddle() -> GetLarge());
                parent -> GetMiddle() -> SetLarge("");
            }
            else
                parent -> SetLarge(parent -> GetMiddle() -> GetSmall());
                
            redist(parent -> GetMiddle(), fullSibling);
        } else {
            cout << "In redist : right error." << endl;
        }
        
    }
}

void Tree::merge(Node* loc, Node* sibling) {
   Node* parent = loc->GetParent();

   if (parent -> GetLeft() == sibling) {
       sibling -> SetLarge(parent -> GetSmall());
       parent -> SetSmall("");
       parent -> swap();
       
       if (sibling -> isLeaf()) {
           parent -> SetMiddle(nullptr);
       }
   
   } else if (parent -> GetMiddle() == sibling) {
       
       if (parent -> GetRight() == loc) {
           sibling -> SetLarge(parent -> GetLarge());
           parent -> SetLarge("");
       
           if (sibling -> isLeaf()) {
               parent -> SetRight(sibling);
               parent -> SetMiddle(nullptr);
           }
       
       } else if (parent -> GetLeft() == loc) {
           sibling -> SetLarge(parent -> GetSmall());
           parent->SetSmall("");
           parent->swap();
           sibling -> swap();
           
           if (sibling -> isLeaf()) {
               parent -> SetLeft(sibling);
               parent -> SetMiddle(nullptr);
           }
       }
       
   } else {
       sibling -> SetLarge(parent -> GetSmall());
       parent -> SetSmall("");
       parent -> swap();
       sibling -> swap();
       
       if (sibling -> isLeaf()) {
           parent -> SetLeft(sibling);
           parent -> SetMiddle(nullptr);
       }
       
   }
   
   if (!loc -> isLeaf()) {
       if (parent -> GetRight() == sibling) {
           sibling -> SetMiddle(sibling -> GetLeft());
           sibling -> SetLeft(loc -> GetRight());
           loc -> GetRight() -> SetParent(sibling);
           parent->SetLeft(nullptr);
       }
   }
    
   delete loc;
   loc = nullptr;

   if ((parent -> GetSmall() == "") && (parent -> GetLarge() == ""))
       fix(parent);
}

void Tree::swapInOrder(Node* removeLoc, Node* suc, const string & str) {
    string temp = str;
    
    if (removeLoc -> GetSmall() == str) { //suc is middle child
        removeLoc -> SetSmall(suc -> GetSmall());
        suc -> SetSmall(temp);
    } else {
        removeLoc -> SetLarge(suc -> GetSmall()); //suc is right child
        suc -> SetSmall(temp);
    }
}
    
void Tree::PrintNode(Node* loc) const {
    if (sizeOne(loc))
        cout << loc -> GetSmall() << ", ";
    else
        cout << loc -> GetSmall() << ", " << loc -> GetLarge() << ", ";
}

void Tree::inOrder() const {
    inOrder(root);
}

void Tree::inOrder(Node* curr) const {
    if (curr) {
        
        inOrder(curr -> GetLeft());
        
            if (curr -> GetSmall() != "")
                cout << curr -> GetSmall() << ", ";
        
        inOrder(curr -> GetMiddle());
        
            if (curr -> GetLarge() != "")
                cout << curr -> GetLarge() << ", ";
        
        inOrder(curr -> GetRight());
        
    }
}

void Tree::postOrder() const {
    postOrder(root);
}

void Tree::postOrder(Node* curr) const {

    if (curr) {
        
        postOrder(curr -> GetLeft());
        postOrder(curr -> GetMiddle());
        
            if (curr -> GetLarge() != "") {
                cout << curr -> GetSmall() << ", ";
                postOrder(curr -> GetRight());
                cout << curr -> GetLarge() << ", ";
            }
            else {
                postOrder(curr -> GetRight());
                cout << curr -> GetSmall() << ", ";
            }
               
    }

}

void Tree::preOrder() const {
    preOrder(root);
}

void Tree::preOrder(Node* curr) const {
    
    if (curr) {
        
        if (curr -> GetSmall() != "") 
            cout << curr -> GetSmall() << ", ";
        
        preOrder(curr -> GetLeft());
        
        if (curr -> GetLarge() != "")
            cout << curr -> GetLarge() << ", ";
        
        preOrder(curr -> GetMiddle());
        preOrder(curr -> GetRight());
         
        
    }
}
