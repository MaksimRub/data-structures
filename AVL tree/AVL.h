#pragma once

typedef struct Node_avl{
    int data;
    struct Node_avl* left;
    struct Node_avl* right;
    int height;
} Node_avl;

Node_avl* insert (Node_avl* root, int data);
Node_avl* del (Node_avl* root, int data);
Node_avl* search (Node_avl* root, int data);
void print (Node_avl* root);

Node_avl* right_rotate (Node_avl* sub_root);
Node_avl* left_rotate (Node_avl* sub_root);
Node_avl* create_node (int data);
Node_avl* get_min_node (Node_avl* sub_root);
int get_height(Node_avl* node);
int get_balance_factor(Node_avl* node);
void update_height (Node_avl* sub_root);
void free_tree (Node_avl* root);
