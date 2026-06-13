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
Node_b* search(Node_b* root, int key){
    if (!root){
        return NULL;
    }
    Node_b* current = root;
    
    while (current){
        int left = binary_search(current, key);
        if (current->keys[left] == key){
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
        new_child->keys[j] = overcrowded_child->keys[j + T];
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
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = overcrowded_child->keys[T - 1];
    parent->num_keys++;
 
}

void insert_non_full(Node_b* node, int key){
    int left = binary_search(node, key);
    if (node->keys[left] == key){
        return;
    }
    

    if (node->is_leaf){
        for (int j = node->num_keys - 1; j >= left; j--) {
            node->keys[j + 1] = node->keys[j];
        }
        node->keys[left] = key;
        node->num_keys++;
    
    }
    else{
        if (node->children[left]->num_keys == MAX_KEYS){
            split_child(node, left);
            if (node->keys[left] < key){
                left++;
            }
        }
        insert_non_full(node->children[left], key);
    }
}

Node_b* insert (Node_b* root, int key){
    if (!root){
        Node_b* new_node = create_node();
        new_node->keys[0] = key;
        new_node->num_keys = 1;
        new_node->is_leaf = 1;
        return new_node;
    }

    if (root->num_keys == MAX_KEYS){
        Node_b* new_node = create_node();
        new_node->children[0] = root;
        split_child(new_node, 0);
        short i = 0;
        if(new_node->keys[0] < key){
            i++;
        }

        insert_non_full(new_node->children[i], key);
        return new_node;
    }
    else{
        insert_non_full(root, key);
        return root;
    }

}
Node_b* get_min_node (Node_b* sub_root);
int binary_search(Node_b* node, int key){
    int right = node->num_keys - 1;
    int left = 0;

    while (left<=right){
            int mid = left + (right - left)/2;
            if (node->keys[mid] < key){
                left = mid + 1;
            }
            else if (node->keys[mid] > key){
                right = mid - 1;
            }
            else{
                return mid;
            }
        }
    return left;
}

Node_b* del (Node_b* root, int key){
    if (!root){
        return NULL;
    }

    delete_internal(root, key);

    if (root->num_keys == 0){
        Node_b* old_root = root;
        root = root->children[0];
        free(old_root);
    }
    return root;
}

void merge_nodes(Node_b* parent, int idx) {
    Node_b* left_child = parent->children[idx];
    Node_b* right_child = parent->children[idx + 1]; 

    left_child->keys[T - 1] = parent->keys[idx];

    for (int j = 0; j < right_child->num_keys; j++){
        left_child->keys[j + T] = right_child->keys[j];
    }
    if (!left_child->is_leaf){
        for (int j = 0; j < right_child->num_keys + 1; j++){
            left_child->children[j + T] = right_child->children[j];
        }
    }

    left_child->num_keys += right_child->num_keys + 1;
    for (int j = idx; j < parent->num_keys - 1; j++){
        parent->keys[j] = parent->keys[j + 1];
    }
    for (int j = idx + 1; j < parent->num_keys; j++){
        parent->children[j] = parent->children[j + 1];
    }
    parent->num_keys--;
    free(right_child);
}

void delete_internal(Node_b* node, int key){
    int idx = binary_search(node, key);
    
    if (idx < node->num_keys && node->keys[idx] == key){
        if (node->is_leaf){
            for (int j = idx; j < node->num_keys - 1; j++){
                node->keys[j] = node->keys[j + 1];
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
                int pred = current->keys[current->num_keys - 1];
                node->keys[idx] = pred;
                delete_internal(pred_node, pred);
            }
            else if (succ_node->num_keys >= T){
                Node_b* current = succ_node;
                while (!current->is_leaf){
                    current = current->children[0]; 
                }
                int succ = current->keys[0]; 
                node->keys[idx] = succ;
                delete_internal(succ_node, succ);
            }
            else{
                merge_nodes(node, idx);
                delete_internal(pred_node, key);
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
        delete_internal(node->children[idx], key);
    }
}

void borrow_from_prev(Node_b* parent, int idx){
    Node_b* left_child = parent->children[idx - 1];
    Node_b* right_child = parent->children[idx]; 

    for (int j = right_child->num_keys - 1; j >= 0; j--){
        right_child->keys[j + 1] = right_child->keys[j];
    }
    if (!right_child->is_leaf){
        for (int j = right_child->num_keys; j >= 0; j--){
            right_child->children[j + 1] = right_child->children[j];
        }
    }

    right_child->keys[0] = parent->keys[idx - 1];

    if (!right_child->is_leaf){
        right_child->children[0] = left_child->children[left_child->num_keys];
        left_child->children[left_child->num_keys] = NULL; 
    }

    parent->keys[idx - 1] = left_child->keys[left_child->num_keys - 1];

    left_child->num_keys--;
    right_child->num_keys++;
}

void borrow_from_next(Node_b* parent, int idx){
    Node_b* left_child = parent->children[idx];
    Node_b* right_child = parent->children[idx + 1];

    left_child->keys[left_child->num_keys] = parent->keys[idx];
    
    
    if (!left_child->is_leaf) {
        left_child->children[left_child->num_keys + 1] = right_child->children[0];
    }

    parent->keys[idx] = right_child->keys[0];

    for (int j = 0; j < right_child->num_keys - 1; j++){
        right_child->keys[j] = right_child->keys[j + 1];
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


Node_b* print(Node_b* root, int level){
    if (root == NULL) {
        printf("Tree is empty.\n");
        return NULL;
    }


    for (int i = 0; i < level; i++) printf("    "); 
    printf("[");
    for (int i = 0; i < root->num_keys; i++) {
        printf("%d", root->keys[i]);
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

void free_tree(Node_b* root){
   if (root == NULL) {
        return;
    }

    if (!root->is_leaf) {
        for (int j = 0; j <= root->num_keys; j++) {
            free_tree(root->children[j]);
        }
    }

    free(root);
    
}

