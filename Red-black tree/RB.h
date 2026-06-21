#pragma once

typedef enum color{
    RED,
    BLACK
}color;

typedef struct Data{
    int key;
}Data;

typedef struct node_rb {
    Data* data;
    color color;
    struct node_rb* left;
    struct node_rb* right;
    struct node_rb* parent;
}node_rb;

void insert (node_rb** root, Data* data);
void del (node_rb** root, Data* data);
node_rb* search (node_rb* root, Data* data);
void print (node_rb* root);
void free_tree (node_rb** root);

void right_rotate (node_rb** root, node_rb* sub_root);
void left_rotate (node_rb** root, node_rb* sub_root);
void fix_grandpa(node_rb** root, node_rb* new_sub_root, node_rb* sub_root);
node_rb* create_node (Data* data, node_rb* parent);
node_rb* get_min_node (node_rb* sub_root);
color get_color (node_rb* node);
void recursive_insert (node_rb** root, Data* data, node_rb* parent, node_rb** new_node);
void fix_insert (node_rb** root, node_rb* new_node);
void recursive_del (node_rb** root, Data* data, node_rb** node_to_del);
void fix_del (node_rb** root, node_rb* x);
char data_compare (Data* in_the_tree, Data* target);
void free_data(Data* data);
void del_without_clean_data(node_rb** root, Data* data, node_rb** node_to_del);
