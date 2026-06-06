#include "BST/BST.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    Node_BST* root = NULL;
    root = insert(root, 5);
    root = insert(root, 3);
    root = insert(root, 7);
    root = insert(root, 2);
    root = insert(root, 4);
    print(root);
    printf("\n");
    root = del(root, 3);
    print(root);
    printf("\n");

    return 0;
}