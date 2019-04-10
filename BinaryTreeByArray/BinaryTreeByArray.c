#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "BinaryTreeByArray.h"

//////////////////////////////////////////////////
// for Binary Tree.
BT_t *createBT(int capacity) {
    // Block illegal parameters.
    if (capacity <= 0) return NULL;
    
    BT_t *B = malloc(sizeof(BT_t));
    if (B == NULL) return NULL;
    B->capacity = capacity;
    B->array = calloc(capacity, sizeof(int));
    if (B->array == NULL) {
        free(B);
        return NULL;
    }
    // initialize an array.
    for (int i=0; i<B->capacity; i++) {
        B->array[i] = -1;
    }
    
    return B;
}

bool destroyNodeBT(BT_t *B) {
    // Block illegal parameters.
    if (B == NULL) return false;
    
    free(B->array);
    free(B);
    
    return true;
}

bool insertElementOnBT(BT_t *B, int value) {
    for (int i=0; i<B->capacity; i++) {
        if (B->array[i] < 0) {
            B->array[i] = value;
            return true;
        }
    }
    return false;
}

bool deleteElementOnBT(BT_t *B, int value) {
    //    Consider a subtree rooted at the node to be deleted.
    //    Replace the deletion node with the leftmost leaf of the subtree.
    int root = 0;
    if (B->array[root] < 0) return false;
    
    int target = findElementIndexOnBT(B, value, BT_OPTION_BREADTH_FIRST_SEARCH);
    if (target < 0) return false;
    int leaf = findLeftmostLeefIndexOnBT(B, target);
    
    // delete node
    if (target == leaf) {
        B->array[target] = -1;
    }
    else {
        B->array[target] = B->array[leaf];
        B->array[leaf] = -1;
    }
    
    return true;
}

int findElementOnBT(BT_t *B, int value, BT_OPTION_e option) {
    int target = findElementIndexOnBT(B, value, option);
    return B->array[target];
}

int findElementIndexOnBT(BT_t *B, int value, BT_OPTION_e option) {
    int root = 0;
    switch (option) {
        case BT_OPTION_BREADTH_FIRST_SEARCH:
            return breadthFirstFindElementIndexOnBT(B, value);
        case BT_OPTION_DEPTH_FIRST_SEARCH:
            root = 0;
            return depthFirstFindElementIndexOnBT(B, root, value);
        default:
            break;
    }
    return -1;
}

int breadthFirstFindElementIndexOnBT(BT_t *B, int value) {
    for (int i=0; i<B->capacity; i++) {
        if (B->array[i] == value) {
            return i;
        }
    }
    return -1;
}

int depthFirstFindElementIndexOnBT(BT_t *B, int root, int value) {
    if (root < 0) return -1;
    if (root >= B->capacity) return -1;
    
    if (B->array[root] == value) return root;
    
    int index = -1;
    index = depthFirstFindElementIndexOnBT(B, getLeftIndex(root), value);
    if (index >= 0) return index;
    index = depthFirstFindElementIndexOnBT(B, getRightIndex(root), value);
    if (index >= 0) return index;
    
    return -1;
}

int findLeftmostLeefIndexOnBT(BT_t *B, int root) {
    if (root < 0) return -1;
    if (root >= B->capacity) return -1;
    
    int left = -1;
    int right = -1;
    left = findLeftmostLeefIndexOnBT(B, getLeftIndex(root));
    if (left < 0) {
        right = findLeftmostLeefIndexOnBT(B, getRightIndex(root));
    }
    return max(max(left, right), root);
}

void levelOrderTraversalOnBT(BT_t *B, int root) {
    for (int i=0; i<B->capacity; i++) {
        printf("level-order traversal : array[%d] = %d\n", i, B->array[i]);
    }
}

void preOrderTraversalOnBT(BT_t *B, int root) {
    if (root < 0) return;
    if (root >= B->capacity) return;
    
    printf("pre-order traversal : array[%d] = %d\n", root, B->array[root]);
    preOrderTraversalOnBT(B, getLeftIndex(root));
    preOrderTraversalOnBT(B, getRightIndex(root));
}

void inOrderTraversalOnBT(BT_t *B, int root) {
    if (root < 0) return;
    if (root >= B->capacity) return;
    
    inOrderTraversalOnBT(B, getLeftIndex(root));
    printf("in-order traversal : array[%d] = %d\n", root, B->array[root]);
    inOrderTraversalOnBT(B, getRightIndex(root));
}

void postOrderTraversalOnBT(BT_t *B, int root) {
    if (root < 0) return;
    if (root >= B->capacity) return;
    
    postOrderTraversalOnBT(B, getLeftIndex(root));
    postOrderTraversalOnBT(B, getRightIndex(root));
    printf("post-order traversal : array[%d] = %d\n", root, B->array[root]);
}

