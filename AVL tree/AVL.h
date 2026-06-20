#pragma once

typedef struct Data{
    int key;
}Data;

typedef struct Node_avl{
    Data* data;
    struct Node_avl* left;
    struct Node_avl* right;
    int height;
} Node_avl;

void insert (Node_avl** root, Data* data);
void del (Node_avl** root, Data* data);
Node_avl* search (Node_avl* root, Data* data);
void print (Node_avl* root);
void free_tree (Node_avl** root);

void right_rotate (Node_avl** sub_root);
void left_rotate (Node_avl** sub_root);
Node_avl* create_node (Data* data);
Node_avl* get_min_node (Node_avl** sub_root);
int get_height(Node_avl* node);
int get_balance_factor(Node_avl* node);
void update_height (Node_avl* sub_root);
void balance (Node_avl** root);
char data_compare (Data* in_the_tree, Data* target);
void free_data(Data* data);
