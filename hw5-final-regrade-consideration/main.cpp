#include <iostream> 
#include <map> 
#include "ArgumentManager.h"
#include <fstream>
#include<cmath> 
#include <iterator>
#include <algorithm>
#include<vector>


using namespace std; 

//notes for insertion: we could have a helper function to check before inserting if the node is full
// split function  

//when inserting if it's the first node in the tree that'll be the root 
// make a helper function to check if tree is empty 

//traverse tree and if not empty choose the right subtree 


//The main idea: 
// each node contains an array of k keys, and pointers to k+1 children. 

//TODO: MAKE A STRUCT NODE
 template<typename T> 
 struct Node {
    T *key; //WILL BE AN INT FOR KEYS to store in array
    T deg; // max value storage
    T keynum = 0; //number of key value ?  (int)
    T w; //number of existing children 
    Node* parent; //make new root 
    Node* search(int k);
    int min; // min number of keys = ceil(m/2) -1 
    int divisor; // index of the split 
    Node** child; //Why double pointer? well instead of doing Node* left, Node* right, we do Node** for both 
    bool isleaf; // to check to see if the node is leaf or not
    bool isFull() {
        if (keynum >= deg-1) {
            return true;
        }
        else {
            return false; 
        }
    } 
    
    Node(int degree) {                //constructor 
        deg = degree;
        keynum = 0; 
        key = new T[degree]; //dynamic array for k keys
        child = new Node*[degree+1]; 
        isleaf = true; 
        divisor = (degree-1)/2;
        min = ceil((degree/2.) - 1); 
        for (int i = 0; i<degree; i++) {
            child[i] = NULL; 
        }
    }

    Node* findNode(Node* temp, T data, T deg, Node* root);
    void split (Node* p, int degrrrr); 


}; 

template<typename T> 
class btree {
    public:
    Node<T> *root;
    int degree; 

    btree(int degre) {   //constructor 
        root = nullptr; 
        degree = degre; 
    }
    Node<T>* getRoot() {
        return root; 
    }
    void traverse(Node<T> *temp,int level ); 
    void insert (T k); //function that inserts a new key in B-tree, when it is full split so the root can have minimum 
    Node<T>* search(int k); //function to search key k in subtree rooted with this node 
    //void sortNode(Node); //we will be using bubble sort to sort the nodes because we are not using a ordered map so we have to implement this ourselves 
     int nodeHeight (Node<T> *p);
     void printLevelorder(int level,ofstream &out); 
     void printLevel(Node<T> *p, int level, int c,ofstream &out); 
     void insertNonFull (int data); 

  //   void split (int val, Node<T>* shift);
};

//All nonleaf nodes(internal nodes) (except the root) have between ceil(M/2)  and M children.

template<typename T>
Node<T>* Node<T>::search(int k) {   //did not use; kept getting a seg fault with 'keynum' 
    int i = 0; 
    while (i< keynum && k> key[i]) {
        i++; 

        if (key[i] == k) {
            return this; 
        }
        if (isleaf) {
            return NULL;
        }
        return child[i]->search(k); 
    }

}
template<typename T> 
Node<T>* btree<T>::search(int k) {
    return root->search(k); //in order to call it in main() 

}
//insert node to tree 
template <typename T> 
void btree<T>::insert(T k) {
    if (root  == NULL) {
        root = new Node<T>(degree); 
        root->isleaf = true; 
        //insert the value
        root->key[0] = k; 
        root->keynum++; 
    }
    else { // the tree isn't empty 
        if (root->keynum == degree) { //if root is full create a new root and split old 
            Node<T>* p = new Node<T>(degree); // allocate new root 
            p->isleaf == false; 
            p->child[0] = root; //make the old root a child of the new root 
            p->split(root,0); //split old root, raise middle value left hand side 
            int j = 0;
            if (p->key[0] < k) {//find which child to put the new val into 
                j++;
            p->child[j]->findNode(p,k,degree,root); }//insert to lhs child 
            //root = p; 
            else  {
                p->child[0]->findNode(p,k,degree,root); //insert to rh child

            }
            root = p; //set root to the new root 
        }
        else {
             root = root->findNode (root,k,degree,root);//if not full then just insert key 

        }
        //root = root->findNode (root,k,degree,root); 
         //root = root->insertNonfull(k); 

    }
}

template <typename T> 
Node<T>* Node<T>::findNode(Node<T>* temp, T data, T deg, Node<T>* temp2) {
    int i = keynum-1; 
    
    if (isleaf) {
        
    while (key[i] > data && i>=0) {
        key[i+1] = key[i];
        i--;
    }
    key[i+1] = data;
    keynum += 1; 
    }
    else {             //root is not a leaf go to the respective child 
        int i =0; 
        while (key[i] < data && i<keynum) {
            i++;
        }
        child[i]->findNode(this,data,deg,temp2); 

    }
    if (keynum== deg) {
        if (this == temp2 ) {
            Node* p = new Node(deg);
            p->isleaf = false; 
            p->child[0] = this; 
            p->split(this,deg);
            return p; 
        }
        else {
            temp->split(this,deg); 
        }


    }
    return temp2; 
}
template<typename T>
void Node<T>::split(Node* right, int degrrrr) {      //sourced by: https://github.com/mayowa-ops/btree-/blob/main/main.cpp
           Node*leftNode=new Node(deg);
        int index=0,move_up=(deg-1)/2;
        //// The nunmber of keys in th4e node at first entry//
        int track=right->keynum;
        int carry=right->key[move_up],i_child=0;
        float val1=ceil(float(deg)/2);
         int walk_child=val1;
        for(int i= move_up+1; i<track; i++){
            leftNode->key[index]=right->key[i];
            index++;
            //fullNode->key[i]=NULL;
            right->keynum=(right->keynum)-1;
            leftNode->keynum=(leftNode->keynum)+1;
        }
        if(right->isleaf==false){
            for(int a=walk_child;a<=deg;a++){
            leftNode->child[i_child]=right->child[a];
            i_child++;
        }
        leftNode->isleaf=right->isleaf;
        }
        int hold=deg-1;
        while(child[hold]!=right){
           child[hold+1]=child[hold];
            hold--;
        }
        child[hold+1]=leftNode;
        int j=deg-1;
            while(!key[j-1] && j!=0){
                j--;
            }
            //// if value before key is greater than the data push it to the right///
            while(key[j-1]>carry && j!=0){
                key[j]=key[j-1];
                j--;
            }
            // we insert the correct values 
            key[j]=carry;
            keynum=keynum+1;
            right->keynum=(right->keynum)-1;
    
}



template<typename T>
int btree<T>::nodeHeight(Node<T> *p) {
    if (p == NULL) {    //if leaf 
            return 0; 
        }
        else {
            return 1 + nodeHeight(p->child[0]); 
        }
}


template<typename T> 
void btree<T>:: printLevelorder(int level,ofstream &out) {

     if (level == 1) {
        for (int i =0; i<root->keynum; i++) {
            out << root->key[i] << " ";
        }
        out <<endl; 
    }
        else {
            printLevel(root,level,1,out); 
            out << endl; 
        }
    }

template<typename T>
void btree<T>::printLevel(Node<T> *p, int level, int c,ofstream &out) {
    if (p == nullptr) {
        return;
    }
    if (level == c) {
        for (int i =0; i<=p->keynum-1; i++) {
            out << p->key[i] << " ";  
        }
    }
    else {
        ++c; 
        for (int i =0; i<= p->keynum; i++) {
            if (p->child[i] != nullptr) {
                printLevel(p->child[i],level,c,out); 
            }
        }
    }
}



//TODO: make main() and probably use an ordered map to sort keys from left to right 
// if not use vector so it sorts or bubble sort? 



int main (int argc, char* argv[]) {  
    ArgumentManager am (argc, argv); 

    ifstream in(am.get("input"));   //uncomment when ready to submit on server
    ifstream com(am.get("command"));  // reading from the command txt file 
    ofstream out(am.get("output")); 


  //  string input = am.get("input"); 
  //  string command = am.get("command"); 
  //  string output = am.get("output");

   // ifstream in ("input52.txt");
   // ifstream com ("command52.txt");
    //ofstream out ("answer.txt");

    int degree; 
    int level; 



    string line; //input 
    int temp; 
    
 string command1; 
 string t_level; 

 
//commands

com >> command1; 
if (command1.find("Degree=")!=string::npos) {
    string condition = command1.substr(7,command1.length()-1);
     degree = stoi(condition);
}
     btree<int> *cl1 = new btree<int>(degree); 
     Node<int> * curr = cl1->root;
     map<int,int>keyPair;


     // input : 
    while (!in.eof()) {
    if (line.length()==0)
    in >> temp;  
    if (!keyPair.count(temp)) {        //everything works, try to see if you can store this in a dynamic array 
        cl1->insert(temp);
        keyPair[temp] = 1; 

    }

    //cout << n; 

}

int height  = cl1->nodeHeight(cl1->root); 
out << "Height=" << height <<endl; 
while (!com.eof()) {
    com >> command1;
    if (isdigit(command1[0])) {
        t_level = command1; 
        level = stoi(t_level);
        if (level > height) {
            out << "Empty" <<endl; 
            continue;
        }

        else {
            cl1->printLevelorder(level,out);
        }

    }

}



    return 0; 
} 