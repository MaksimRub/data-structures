#pragma once
#define T 2
#define MAX_KEYS (2*T - 1)
#define MAX_CHILDREN (2*T)

typedef struct Node_b{
    int keys[MAX_KEYS];
    struct Node_b* children [MAX_CHILDREN];
    short is_leaf : 2;
    int num_keys;
}Node_b;

Node_b* insert (Node_b* root, int key);
Node_b* del (Node_b* root, int key);
Node_b* search(Node_b* root, int key);
Node_b* print(Node_b* root, int level);

Node_b* create_node ();
void delete_internal(Node_b* node, int key); 
Node_b* get_min_node (Node_b* sub_root);
void split_child(Node_b* parent, int i);
void insert_non_full(Node_b* node, int key);
int binary_search(Node_b* node, int key);
void merge_nodes(Node_b* parent, int idx);
void borrow_from_prev(Node_b* parent, int idx);
void borrow_from_next(Node_b* parent, int idx);
void free_tree(Node_b* root);



