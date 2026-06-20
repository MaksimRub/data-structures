#include "RB.h"
#include "stdlib.h"
#include "stdio.h"
void fix_grandpa(node_rb** root, node_rb* new_sub_root, node_rb* sub_root){
    if (new_sub_root->parent == NULL){
        *root = new_sub_root;
    }
    else if (new_sub_root->parent->left == sub_root){
        new_sub_root->parent->left = new_sub_root;
    }
    else{
        new_sub_root->parent->right = new_sub_root;
    }
}

void right_rotate (node_rb** root, node_rb* sub_root){
    node_rb* new_sub_root = sub_root->left;

    sub_root->left = new_sub_root->right;
    new_sub_root->right = sub_root;

    if (sub_root->left) sub_root->left->parent = sub_root;
    new_sub_root->parent = sub_root->parent;
    sub_root->parent = new_sub_root;

    fix_grandpa(root, new_sub_root, sub_root);
}
void left_rotate (node_rb** root, node_rb* sub_root){
    node_rb* new_sub_root = sub_root->right;

    sub_root->right = new_sub_root->left;
    new_sub_root->left = sub_root;

    if (sub_root->right) sub_root->right->parent = sub_root;
    new_sub_root->parent = sub_root->parent;
    sub_root->parent = new_sub_root;

    fix_grandpa(root, new_sub_root, sub_root);
}
node_rb* create_node (int data, node_rb* parent){
    node_rb* new_node = (node_rb*) malloc(sizeof(node_rb));
    if (!new_node){
        printf ("error with memmory allocation\n");
        exit(1);
    }
    new_node->parent = parent;
    new_node->color = RED;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data = data;

    return new_node;
}
node_rb* get_min_node (node_rb* sub_root){
    while (sub_root && sub_root->left){
        sub_root = sub_root->left;
    }
    return sub_root;
}
color get_color (node_rb* node){
    return node ? node->color : BLACK;
}

void print (node_rb* root){
    if (!root){
        return;
    }
    print(root->left);
    printf("%d ", root->data);
    print(root->right);
}
void free_tree (node_rb* root){
    if (!root){
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free (root);
}

node_rb* insert (node_rb* root, int data){
    node_rb* new_node = NULL;
    root = recursive_insert(root, data, NULL, &new_node);
    if (new_node){
        fix_insert(&root, new_node);
    }

    return root;
}

void fix_insert (node_rb** root, node_rb* x){
    while (x->parent && get_color(x->parent) == RED){
        node_rb* p = x->parent;
        node_rb* g = p->parent;

        if (p == g->left){
            node_rb* u = g->right;
            if (get_color(u) == RED){
                u->color = BLACK;
                p->color = BLACK;
                g->color = RED;
                x = g;
            }
            else{
                if (x == p->right){
                    x = p;
                    left_rotate(root, x);
                    p = x->parent;
                }
                right_rotate(root, g);
                p->color = BLACK;
                g->color = RED;
            }
        }
        else{
            node_rb* u = g->left;
            if (get_color(u) == RED){
                u->color = BLACK;
                p->color = BLACK;
                g->color = RED;
                x = g;
            }
            else{
                if (x == p->left){
                    x = p;
                    right_rotate(root, x);
                    p = x->parent;
                }
                left_rotate(root, g);
                p->color = BLACK;
                g->color = RED;
            }
        }
    }

    (*root)->color = BLACK;
}

node_rb* recursive_insert (node_rb* root, int data, node_rb* parent, node_rb** new_node){
    if (!root){
        *new_node = create_node(data, parent);
        return *new_node;
    }
    if (data < root->data){
        root->left = recursive_insert(root->left, data, root, new_node);
    }
    else if (data > root->data){
        root->right = recursive_insert(root->right, data, root, new_node);
    }
    return root;
}
node_rb* del (node_rb* root, int data){
    node_rb* node_to_del = NULL;
    root = recursive_del(root, data, &node_to_del);
    if (node_to_del){
        fix_del(&root, node_to_del);

        node_rb* p = node_to_del->parent;
        if (p){
            if (p->left == node_to_del) p->left = NULL;
            else p->right = NULL;
        }
        else{
            root = NULL;
        }
        free(node_to_del);
    }

        
    

    return root;

}
void fix_del (node_rb** root, node_rb* x){
    while (x != *root && get_color(x) == BLACK){
        node_rb* p = x->parent;
        node_rb* s = NULL;
        if (p->left == x){
            s = p->right;
            if (get_color(s) == RED){
                s->color = BLACK;
                p->color = RED;
                left_rotate(root, p);
                s = p->right;
            }
            if (get_color(s->left) == BLACK && get_color(s->right) == BLACK){
                s->color = RED;
                x = p;
            }
            else{
                if (get_color(s->right) == BLACK){
                    s->left->color = BLACK;
                    s->color = RED;
                    right_rotate(root, s);
                    s = p->right;
                }
                s->color = p->color;
                p->color = BLACK;
                s->right->color = BLACK;
                left_rotate(root, p);
                x = *root;
            }
        }
        else{
            s = p->left;
            if (get_color(s) == RED){
                p->color = RED;
                s->color = BLACK;
                right_rotate(root, p);
                s = p->left;
            }

            if (get_color(s->left) == BLACK && get_color(s->right) == BLACK){
                s->color = RED;
                x = p;
            }
            else{
                if (get_color(s->left) == BLACK){
                    s->color = RED;
                    s->right->color = BLACK;
                    left_rotate(root,s);
                    s = p->left;
                }

                s->color = p->color;
                p->color = BLACK;
                s->left->color = BLACK;
                right_rotate(root, p);
                x = *root;
            }
        }

    }

    if (x != NULL) {
        x->color = BLACK;
    }
}
node_rb* recursive_del (node_rb* root, int data, node_rb** node_to_del){
    if (!root){
        return NULL;
    }
    if (root->data < data){
        root->right = recursive_del(root->right, data, node_to_del);
    }
    else if (root->data > data){
        root->left = recursive_del(root->left, data, node_to_del);
    }
    else{
        if (root->left && root->right){
            node_rb* min_node  = get_min_node(root->right);
            root->data = min_node->data;
            root->right = recursive_del(root->right, min_node->data, node_to_del);
        }
        else{
            node_rb* temp = root->left ? root->left : root->right;
            if (temp){
                temp->parent = root->parent;
                temp->color = BLACK;
                free(root);
                return temp;
            }
            else{
                if (get_color(root) == RED){
                    free(root);
                    return NULL;
                }
                else{
                    *node_to_del = root;
                }
            }
        }
    }
    return root;
}
node_rb* search (node_rb* root, int data){
    node_rb* cur = root;
    while (cur){
        if (root->data < data){
            cur = root->right;
        }
        else if (root->data > data){
            cur = root->left;
        }
        else{
            return cur;
        }
    }
    return NULL;
}