#include "BST.h"
#include <stdio.h>
#include <stdlib.h>

void print(Node_BST *root){
    if (root != NULL){
        print(root->left);
        printf("%d ", root->data);
        print(root->right);
    }
}
Node_BST* create_node_BST(int data){
    Node_BST* new_node = (Node_BST*) malloc(sizeof(Node_BST));
        if (new_node == NULL){
            printf("error memory allocation \n");
            exit(1);
        }
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
}

Node_BST* minValueNode(Node_BST* node){
    while (node && node->left != NULL){
        node = node->left;
    }
    return node;
}
Node_BST* insert(Node_BST *root, int data ){
    if (root == NULL){
        return create_node_BST(data);
    }
    if (root->data > data){
        root->left = insert(root->left, data);
    }
    if (root->data < data){
        root->right = insert(root->right, data);
    }
    return root;
}

Node_BST* del(Node_BST *root, int data){
    if (root == NULL){
        return root;
    }
    if (root->data > data){
        root->left = del(root->left, data);
    }
    else if (root->data < data){
        root->right = del(root->right, data);
    }
    else{
        if (root->left == NULL){
            Node_BST* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL){
            Node_BST* temp = root->left;
            free(root);
            return temp;
        }
        else{
            Node_BST* to_del_leaf = minValueNode(root->right);
            root->data = to_del_leaf->data;
            root->right = del(root->right, to_del_leaf->data);
        }
    }
    return root;
}

Node_BST* search(Node_BST *root, int data){
    Node_BST* current = root;
    
    while (current != NULL) {
        if (current->data == data) {
            return current; // Элемент найден
        }
        if (current->data > data) {
            current = current->left;  // Спускаемся влево
        } else {
            current = current->right; // Спускаемся вправо
        }
    }
    
    return NULL;
}

void free_tree (Node_BST* root){
    if(!root){
        return;
    }
    free_tree (root->left);
    free_tree(root->right);
    
    free(root);
}