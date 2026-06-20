#include "BST.h"
#include <stdio.h>
#include <stdlib.h>

void print(Node_BST *root){
    if (root != NULL){
        print(root->left);
        printf("%d ", root->data->key);
        print(root->right);
    }
}
void free_data(Data* data){
    //to realise if it need
}
char data_compare (Data* in_the_tree, Data* target){
    if (in_the_tree->key < target->key){
        return 1;
    }
    else if (in_the_tree->key > target->key){
        return -1;
    }
    else{
        return 0;
    }
}

Node_BST* create_node_BST(Data* data){
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

Node_BST** minValueNode(Node_BST** node){
    while (*node && (*node)->left != NULL){
        node = &(*node)->left;
    }
    return node;
}
void insert(Node_BST **root, Data* data ){
    if (*root == NULL){
        *root = create_node_BST(data);
        return;
    }
    char cmp = data_compare((*root)->data, data);
    if (cmp < 0){
        insert(&(*root)->left, data);
    }
    else if (cmp > 0){
        insert(&(*root)->right, data);
    }
}

void del(Node_BST **root, Data* data){
    if (*root == NULL){
        return;
    }
    char cmp = data_compare((*root)->data, data);
    if (cmp < 0){
        del(&(*root)->left, data);
    }
    else if (cmp > 0){
        del(&(*root)->right, data);
    }
    else{
        if ((*root)->left == NULL){
            Node_BST* temp = (*root)->right;
            free_data((*root)->data);
            free((*root)->data);
            free(*root);
            *root = temp;
            return;
        }
        else if ((*root)->right == NULL){
            Node_BST* temp = (*root)->left;
            free_data((*root)->data);
            free((*root)->data);
            free(*root);
            *root = temp;
            return;
        }
        else{
            Node_BST** receiver_ptr = minValueNode(&(*root)->right);
            Node_BST* receiver = *receiver_ptr;

            *receiver_ptr = receiver->right; //give receiver's right child to receiver's parent

            receiver->left = (*root)->left;
            receiver->right = (*root)->right;

            Node_BST* to_free = *root;
            *root = receiver;
            free_data(to_free->data);
            free(to_free->data);
            free(to_free);


        }
    }
}

Node_BST* search(Node_BST *root, Data* data){
    Node_BST* current = root;
    char cmp;

    while (current != NULL) {
        cmp = data_compare(current->data, data);
        if (cmp == 0) {
            return current; 
        }
        if (cmp < 0) {
            current = current->left;  
        } else {
            current = current->right; 
        }
    }
    
    return NULL;
}

void free_tree (Node_BST** root){
    if(!*root){
        return;
    }
    free_tree (&(*root)->left);
    free_tree (&(*root)->right);
    
    free_data((*root)->data);
    free((*root)->data);
    free(*root);
    *root = NULL;
}