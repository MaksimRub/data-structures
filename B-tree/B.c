#include "B.h"
#include "stdio.h"
#include "stdlib.h"

Node_b* create_node (){
    Node_b* new_node = (Node_b*) calloc(1, sizeof(Node_b));
    if(!new_node){
        printf("error with memmory allocation\n");
        exit(1);
    }
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
Node_b* search(Node_b* root, Data* data){
    if (!root){
        return NULL;
    }
    Node_b* current = root;
    
    while (current){
        int left = binary_search(current, data);
        if (data_compare(current->data[left], data) == 0){
            return current;
        }

        if (current->is_leaf){
            break;
        }
        current = current->children[left];
    }
    
    return NULL;

}

void split_child(Node_b* parent, int i){
    Node_b* overcrowded_child = parent->children[i];
    Node_b* new_child = create_node();

    new_child->is_leaf = overcrowded_child->is_leaf;

    new_child->num_keys = T - 1;

    for (int j = 0; j < T - 1; j++){
        new_child->data[j] = overcrowded_child->data[j + T];
    }
    if (!new_child->is_leaf){
        for (int j = 0; j < T; j++){
        new_child->children[j] = overcrowded_child->children[j + T];
        overcrowded_child->children[j + T] = NULL;
        }
    }

    overcrowded_child->num_keys = T - 1;

    for (int j = parent->num_keys; j >= i + 1; j--){
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[i + 1] = new_child;

    for (int j = parent->num_keys - 1; j >= i; j--){
        parent->data[j + 1] = parent->data[j];
    }
    parent->data[i] = overcrowded_child->data[T - 1];
    parent->num_keys++;
 
}

void insert_non_full(Node_b* node, Data* data){
    int left = binary_search(node, data);
    if (left < node->num_keys && data_compare(node->data[left], data) == 0){
        return;
    }
    

    if (node->is_leaf){
        for (int j = node->num_keys - 1; j >= left; j--) {
            node->data[j + 1] = node->data[j];
        }
        node->data[left] = data;
        node->num_keys++;
    
    }
    else{
        if (node->children[left]->num_keys == MAX_KEYS){
            split_child(node, left);
            if (left < node->num_keys && data_compare(node->data[left], data) > 0){
                left++;
            }
        }
        insert_non_full(node->children[left], data);
    }
}

void insert (Node_b** root,  Data* data){
    if (!(*root)){
        Node_b* new_node = create_node();
        new_node->data[0] = data;
        new_node->num_keys = 1;
        new_node->is_leaf = 1;
        *root = new_node;
        return;
    }

    if ((*root)->num_keys == MAX_KEYS){
        Node_b* new_node = create_node();
        new_node->children[0] = *root;
        split_child(new_node, 0);
        short i = 0;
        if(data_compare(new_node->data[0], data) > 0){
            i++;
        }

        insert_non_full(new_node->children[i], data);
        *root = new_node;
    }
    else{
        insert_non_full(*root, data);
    }

}
int binary_search(Node_b* node, Data* data){
    int right = node->num_keys - 1;
    int left = 0;
    char cmp;
    int mid;
    while (left<=right){
            mid = left + (right - left)/2;
            cmp = data_compare(node->data[mid], data); 
            if (cmp > 0){
                left = mid + 1;
            }
            else if (cmp < 0){
                right = mid - 1;
            }
            else{
                return mid;
            }
        }
    return left;
}

void del (Node_b** root, Data* data){
    if (!*root){
        return;
    }

    delete_internal(*root, data);

    if ((*root)->num_keys == 0){
        Node_b* old_root = *root;
        *root = (*root)->children[0];
        free(old_root);
    }
    return;
}

void merge_nodes(Node_b* parent, int idx) {
    Node_b* left_child = parent->children[idx];
    Node_b* right_child = parent->children[idx + 1]; 

    left_child->data[T - 1] = parent->data[idx];

    for (int j = 0; j < right_child->num_keys; j++){
        left_child->data[j + T] = right_child->data[j];
    }
    if (!left_child->is_leaf){
        for (int j = 0; j < right_child->num_keys + 1; j++){
            left_child->children[j + T] = right_child->children[j];
        }
    }

    left_child->num_keys += right_child->num_keys + 1;
    for (int j = idx; j < parent->num_keys - 1; j++){
        parent->data[j] = parent->data[j + 1];
    }
    for (int j = idx + 1; j < parent->num_keys; j++){
        parent->children[j] = parent->children[j + 1];
    }
    parent->num_keys--;
    free(right_child);
}
void del_without_clean_data(Node_b* node, Data* data){
    int idx = binary_search(node, data);
    
    if (idx < node->num_keys && data_compare(node->data[idx], data) == 0){
        for (int j = idx; j < node->num_keys - 1; j++){
            node->data[j] = node->data[j + 1];
        }
        node->num_keys--;
    }
    else{
        if (node->is_leaf) return;
        Node_b* child = node->children[idx];
        if (child->num_keys == T - 1){
            if (idx > 0 && node->children[idx - 1]->num_keys >= T){
                borrow_from_prev(node, idx);
            }
            else if (idx < node->num_keys && node->children[idx + 1]->num_keys >= T){
                borrow_from_next(node, idx);
            }
            else{
                if (idx < node->num_keys) merge_nodes(node, idx);
                else {
                    merge_nodes(node, idx - 1);
                    idx--;
                }
            }
        }
        del_without_clean_data(node->children[idx], data);
    }
}
void delete_internal(Node_b* node, Data* data){
    int idx = binary_search(node, data);
    if (idx < node->num_keys && data_compare(node->data[idx], data) == 0){
        if (node->is_leaf){
            free_data(node->data[idx]);
            free(node->data[idx]);
            for (int j = idx; j < node->num_keys - 1; j++){
                node->data[j] = node->data[j + 1];
            }
            node->num_keys--;
        }
        else{
            Node_b* pred_node = node->children[idx];
            Node_b* succ_node = node->children[idx + 1];

            if (pred_node->num_keys >= T){
                Node_b* current = pred_node;
                while (!current->is_leaf){
                    current = current->children[current->num_keys];
                }
                Data* pred = current->data[current->num_keys - 1];
                free_data(node->data[idx]);
                free(node->data[idx]);
                node->data[idx] = pred;
                del_without_clean_data(pred_node, pred);
            }
            else if (succ_node->num_keys >= T){
                Node_b* current = succ_node;
                while (!current->is_leaf){
                    current = current->children[0]; 
                }
                Data* succ = current->data[0]; 
                free_data(node->data[idx]);
                free(node->data[idx]);
                node->data[idx] = succ;
                del_without_clean_data(succ_node, succ);
            }
            else{
                merge_nodes(node, idx);
                delete_internal(pred_node, data);
            }
        }
    }
    else{
        if (node->is_leaf) return;
        Node_b* child = node->children[idx];
        if (child->num_keys == T - 1){
            if (idx > 0 && node->children[idx - 1]->num_keys >= T){
                borrow_from_prev(node, idx);
            }
            else if (idx < node->num_keys && node->children[idx + 1]->num_keys >= T){
                borrow_from_next(node, idx);
            }
            else{
                if (idx < node->num_keys) merge_nodes(node, idx);
                else {
                    merge_nodes(node, idx - 1);
                    idx--;
                }
            }
        }
        delete_internal(node->children[idx], data);
    }
}

void borrow_from_prev(Node_b* parent, int idx){
    Node_b* left_child = parent->children[idx - 1];
    Node_b* right_child = parent->children[idx]; 

    for (int j = right_child->num_keys - 1; j >= 0; j--){
        right_child->data[j + 1] = right_child->data[j];
    }
    if (!right_child->is_leaf){
        for (int j = right_child->num_keys; j >= 0; j--){
            right_child->children[j + 1] = right_child->children[j];
        }
    }

    right_child->data[0] = parent->data[idx - 1];

    if (!right_child->is_leaf){
        right_child->children[0] = left_child->children[left_child->num_keys];
        left_child->children[left_child->num_keys] = NULL; 
    }

    parent->data[idx - 1] = left_child->data[left_child->num_keys - 1];

    left_child->num_keys--;
    right_child->num_keys++;
}

void borrow_from_next(Node_b* parent, int idx){
    Node_b* left_child = parent->children[idx];
    Node_b* right_child = parent->children[idx + 1];

    left_child->data[left_child->num_keys] = parent->data[idx];
    
    
    if (!left_child->is_leaf) {
        left_child->children[left_child->num_keys + 1] = right_child->children[0];
    }

    parent->data[idx] = right_child->data[0];

    for (int j = 0; j < right_child->num_keys - 1; j++){
        right_child->data[j] = right_child->data[j + 1];
    }

    if (!right_child->is_leaf){
        for (int j = 0; j < right_child->num_keys; j++){
            right_child->children[j] = right_child->children[j + 1];
        }
        right_child->children[right_child->num_keys] = NULL;
    }

    right_child->num_keys--;
    left_child->num_keys++;
}


void print(Node_b* root, int level){
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }


    for (int i = 0; i < level; i++) printf("    "); 
    printf("[");
    for (int i = 0; i < root->num_keys; i++) {
        printf("%d", root->data[i]->key);
        if (i < root->num_keys - 1) printf(", ");
    }
    printf("] (%s, keys: %d)\n", root->is_leaf ? "Leaf" : "Internal", root->num_keys);

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            if (root->children[i] != NULL) {
                print(root->children[i], level + 1);
            }
        }
    }
}

void free_tree(Node_b** root){
   if (*root == NULL) {
        return;
    }

    if (!(*root)->is_leaf) {
        for (int j = 0; j <= (*root)->num_keys; j++) {
            free_tree(&(*root)->children[j]);
        }
    }
    
    for (int i = 0; i < (*root)->num_keys; i++)    {
        free_data((*root)->data[i]);
        free((*root)->data[i]);
    }
    free(*root);

    *root = NULL;
    
}

