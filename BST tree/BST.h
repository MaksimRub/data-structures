#pragma once
#include <stdbool.h>

typedef struct Data{
    int key;
}Data;

typedef struct Node_BST{
    Data* data;
    struct Node_BST* left;
    struct Node_BST* right;
}Node_BST;

void print(Node_BST *root);
void insert(Node_BST **root, Data* data);
void del (Node_BST **root, Data* data);
Node_BST* search(Node_BST *root, Data* data);
void free_tree(Node_BST** root);

Node_BST** minValueNode(Node_BST** node);
Node_BST* create_node_BST(Data* data);
char data_compare (Data* in_the_tree, Data* target);
void free_data(Data* data);

