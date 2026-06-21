#pragma once
#define T 2
#define MAX_KEYS (2*T - 1)
#define MAX_CHILDREN (2*T)

typedef struct Data{
    int key;
}Data;


typedef struct Node_b{
    Data* data[MAX_KEYS];
    struct Node_b* children [MAX_CHILDREN];
    short is_leaf : 2;
    int num_keys;
}Node_b;

void insert (Node_b** root, Data* data);
void del (Node_b** root, Data* data);
Node_b* search(Node_b* root, Data* data);
void print(Node_b* root, int level);
void free_tree(Node_b** root);

Node_b* create_node ();
void delete_internal(Node_b* node, Data* data); 
void split_child(Node_b* parent, int i);
void insert_non_full(Node_b* node, Data* data);
int binary_search(Node_b* node, Data* data);
void merge_nodes(Node_b* parent, int idx);
void borrow_from_prev(Node_b* parent, int idx);
void borrow_from_next(Node_b* parent, int idx);
char data_compare (Data* in_the_tree, Data* target);
void free_data(Data* data);



