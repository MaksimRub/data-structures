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
Node_avl* right_rotate (Node_avl* sub_root){
    Node_avl* temp = sub_root->left; 
    sub_root->left = temp->right;
    temp->right = sub_root;
    update_height(sub_root);
    update_height(temp);
    return temp;
}
Node_avl* left_rotate (Node_avl* sub_root){
    Node_avl* temp = sub_root->right; 
    sub_root->right = temp->left;
    temp->left = sub_root;
    update_height(sub_root);
    update_height(temp);
    return temp;
}
Node_avl* create_node (int data){
    Node_avl* new_node = (Node_avl*) malloc(sizeof(Node_avl));
    if(!new_node){
        printf("error mem\n");
        exit(1);
    }
    new_node->height = 1;
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
}

Node_avl* insert (Node_avl* root, int data){
    if (!root){
        return create_node(data);
    }
    if (root->data > data){
        root->left = insert(root->left, data);
    }
    else if (root->data < data){
        root->right = insert(root->right, data);
    }
    else{
        return root;
    }

    update_height(root);

    int balance_factor_root = get_balance_factor(root);
    if (balance_factor_root == 2){
        if (get_balance_factor(root->right) >= 0){
            return left_rotate(root);
        }
        else{
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }
    }
    if (balance_factor_root == -2){
        if (get_balance_factor(root->left) <= 0){
            return right_rotate(root);
        }
        else{
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }
    }

    return root;
}

Node_avl* get_min_node (Node_avl* sub_root){
    while(sub_root && sub_root->left){
        sub_root = sub_root->left;
    }
    return sub_root;
}

Node_avl* del (Node_avl* root, int data){
    if (!root){
        return NULL;
    }
    if (root->data > data){
        root->left = del(root->left, data);
    }
    else if (root->data < data){
        root->right = del(root->right, data);
    }
    else{
        if (!root->left){
            Node_avl* temp = root->right;
            free(root);
            return temp;
        }
        else if (!root->right){
            Node_avl* temp = root->left;
            free(root);
            return temp;
        }
        else{
            Node_avl* temp = get_min_node(root->right);
            root->data = temp->data;
            root->right = del(root->right, temp->data);
        }
    }

    update_height(root);

    int balance_factor_root = get_balance_factor(root);

    if (balance_factor_root == 2){
        if (get_balance_factor(root->right) >=0){
            return left_rotate(root);
        }
        else{
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }
    }
    if (balance_factor_root == -2){
        if (get_balance_factor(root->left) <=0){
            return right_rotate(root);
        }
        else{
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }
    }

    return root;

}
Node_avl* search (Node_avl* root, int data){
    if (!root){
        return NULL;
    }
    if (root->data > data){
        root->left = del(root->left, data);
    }
    else if (root->data < data){
        root->right = del(root->right, data);
    }
    else{
        return root;
    }
}
Node_avl* print (Node_avl* root, int data);