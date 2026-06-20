#include "AVL.h"
#include "stdlib.h"
#include <stdio.h>
int get_height(Node_avl* node){
    return node ? node->height : 0;
}
int get_balance_factor(Node_avl* node){
    return node ? get_height(node->right) - get_height(node->left) : 0;
} 
int max(int a, int b){
    return a > b ? a : b;
}
void update_height (Node_avl* sub_root){
    if (sub_root) {
        sub_root->height = max(get_height(sub_root->left), get_height(sub_root->right)) + 1;
    }
}
void right_rotate (Node_avl** sub_root){
    Node_avl* temp = (*sub_root)->left; 
    (*sub_root)->left = temp->right;
    temp->right = *sub_root;
    update_height(*sub_root);
    update_height(temp);
    *sub_root = temp;
}
void left_rotate (Node_avl** sub_root){
    Node_avl* temp = (*sub_root)->right; 
    (*sub_root)->right = temp->left;
    temp->left = *sub_root;
    update_height(*sub_root);
    update_height(temp);
    *sub_root = temp;
}
Node_avl* create_node (Data* data){
    Node_avl* new_node = (Node_avl*) malloc(sizeof(Node_avl));
    if(!new_node){
        printf("error mem\n");
        exit(1);
    }
    new_node->height = 1;
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
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

void free_data(Data* data){
    //to realise if it need
}

void balance (Node_avl** root){
    if (*root == NULL) return;

    update_height(*root);

    int balance_factor_root = get_balance_factor(*root);
    if (balance_factor_root == 2){
        if (get_balance_factor((*root)->right) >= 0){
            left_rotate(root);
        }
        else{
            right_rotate(&(*root)->right);
            left_rotate(root);
        }
    }
    if (balance_factor_root == -2){
        if (get_balance_factor((*root)->left) <= 0){
            right_rotate((root))    ;
        }
        else{
            left_rotate(&(*root)->left);
            right_rotate((root));
        }
    }
}

void insert (Node_avl** root, Data* data){
    if (!(*root)){
        *root = create_node(data);
        return;
    }
    char cmp = data_compare((*root)->data, data);
    if ( cmp < 0){
        insert(&((*root)->left), data);
    }
    else if ( cmp > 0 ){
        insert(&((*root)->right), data);
    }
    else{
        return;
    }

    balance(root);
}

Node_avl* get_min_node (Node_avl** sub_root){
    Node_avl* current = *sub_root;
    if ((*sub_root)->left == NULL){
        *sub_root = current->right;
        return current;
    }

    current = get_min_node(&current->left);
    balance(sub_root);
    return current;
    
}   
void del (Node_avl** root, Data* data){
    if (!*root){
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
        if (!(*root)->left){
            Node_avl* temp = (*root)->right;
            free_data((*root)->data);
            free((*root)->data);
            free(*root);
            *root = temp;
        }
        else if (!(*root)->right){
            Node_avl* temp = (*root)->left;
            free_data((*root)->data);
            free((*root)->data);
            free(*root);
            *root = temp;
        }
        else{
            Node_avl* receiver = get_min_node(&(*root)->right);
            receiver->left = (*root)->left;
            receiver->right = (*root)->right;
            Node_avl* temp = *root;
            *root = receiver;
            free_data(temp->data);
            free(temp->data);
            free(temp);
        }
    }

    balance(root);

}
Node_avl* search (Node_avl* root, Data* data){
    Node_avl* current = root;
    
    while (current != NULL) {
        char cmp = data_compare(current->data, data);
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
void print (Node_avl* root){
    if (!root){
        return;
    }
    print(root->left);
    printf("%d ", root->data->key); // realise print_data if it need
    print(root->right);
}
void free_tree (Node_avl** root){
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