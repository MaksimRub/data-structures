#pragma once


typedef struct Node_BST{
    int data;
    struct Node_BST* left;
    struct Node_BST* right;
}Node_BST;

void print(Node_BST *root);
Node_BST* insert(Node_BST *root, int data);
Node_BST* del (Node_BST *root, int data);
Node_BST* search(Node_BST *root, int data);
void free_tree(Node_BST* root);

Node_BST* minValueNode(Node_BST* node);
Node_BST* create_node_BST(int data);

