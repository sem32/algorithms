#include <memory.h>
#include <stdio.h>
#include "splay.h"



void SplayTree::add(int a)
{
    if (root == NULL)
    {
        root = new Node(a);
        return;
    }

    Node* p = root;

    while (p != NULL)
    {
        if (a > p->data)
        {
            if (p->right == NULL)
            {
                p->right = new Node(a, p);
                break;
            }
            else
            {
                p = p->right;
            }
        }
        else if (a < p->data)
        {
            if (p->left == NULL)
            {
                p->left = new Node(a, p);
                break;
            }
            else
            {
                p = p->left;
            }
        }
        else
        {
            break;
        }
    }
}

int SplayTree::get(int a)
{
    Node* p = find(a);

    splay(p);

    return (p == NULL) ? -1 : p->data;
}

void SplayTree::check()
{
    inOrderPrint(root);
}

Node* SplayTree::find(int a)
{
    Node* p = root;

    while (p != NULL)
    {
        if (a > p->data)
        {
            p = p->right;
        }
        else if (a < p->data)
        {
            p = p->left;
        }
        else
        {
            break;
        }
    }
    return p;
}

void SplayTree::inOrderPrint(Node *p)
{
    if (p == NULL)
    {
        return;
    }
    inOrderPrint(p->left);
    printf("%d ", p->data);
    inOrderPrint(p->right);
}







void SplayTree::rotate(Node* p)
{
    if (p == NULL)
    {
        return;
    }

    int isLeft = -1;

    Node* x = p;
    Node* y = p->parent;
    Node* z = (p->parent != NULL) ? p->parent->parent : NULL;

    if (z != NULL)
    {
        isLeft = (z->left == y) ? 1 : 0;
    }

    // 1
    if (y == NULL)
    {
        return;
    }
    else
    {
        if (x == y->left) // We are left son
        {
            y->left = x->right;
            if (x->right)
                x->right->parent = y;
            x->right = y;
            x->parent = y->parent;
            y->parent = x;
        }
        else  // We are right son
        {
            y->right = x->left;

            if (x->left)
                x->left->parent = y;

            x->left = y;
            x->parent = y->parent;
            y->parent = x;
        }

        if (isLeft >= 0)
        {
            if (isLeft == 1)
            {
                z->left = x;
            }
            else
            {
                z->right = x;
            }
        }
    }
}

void SplayTree::splay(Node *p)
{
    if (p == NULL)
    {
        return;
    }

    Node* x = p;

    while (1)
    {
        if (x->parent == NULL)
        {
            root = x;
            break;
        }
        else
        {
            rotate(x);
        }
    }
}
