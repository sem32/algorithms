#ifndef SPLAY_H
#define SPLAY_H

class Node
{
public:
    Node(int a) { data = a; left = right = parent = NULL; }
    Node(int a, Node* p) { data = a; left = right = NULL; parent = p; }
    ~Node() { data = -1; left = right = parent = NULL; }

    int data;

    Node* left;
    Node* right;
    Node* parent;
};


class SplayTree
{
public:
    SplayTree() { root = NULL; }
    ~SplayTree() { };

    void  add(int a);
    int   get(int a);
    void  check();

private:
    Node* root;

    Node* find(int a);
    void splay(Node* p);
    void inOrderPrint(Node* p);
    void rotate(Node *p);
};


#endif // SPLAY_H
