#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "BinaryTreeByArray.h"

//////////////////////////////////////////////////
#define getParent(v) ((v - 1) / 2)
#define getLeftIndex(v) (v * 2 + 1)
#define getRightIndex(v) (v * 2 + 2)
#define max(a, b) (a > b ? a : b)

//////////////////////////////////////////////////
//  private
int findElementIndexOnBT(BT_t *B, int rootIndex, int keyValue, BT_OPTION_e option);
int findLeftmostLeefIndexOnBT(BT_t *B, int root);
int findElementIndexOnBTslave(BT_t *B, int nodeIndex, void *keyValue);

//////////////////////////////////////////////////
//  public
BT_t *createBT(int capacity) {
    // Block illegal parameters.
    if (capacity <= 0) return NULL;
    
    BT_t *B = malloc(sizeof(BT_t));
    if (B == NULL) return NULL;
    B->capacity = capacity;
    B->array = calloc(capacity, sizeof(BTN_t**));
    if (B->array == NULL) {
        free(B);
        return NULL;
    }
    // initialize an array.
    for (int i=0; i<B->capacity; i++) {
        B->array[i] = NULL;
    }
    
    return B;
}

bool destroyBT(BT_t *B, BT_OPTION_e option) {
    // Block illegal parameters.
    if (B == NULL) return false;
    
    for (int i=0; i<B->capacity; i++) {
        if (B->array[i] != NULL) {
            if (option == BT_OPTION_WITH_ELEMENT) {
                if (B->array[i]->element != NULL) {
                    free(B->array[i]->element);
                }
            }
            free(B->array[i]);
        }
    }
    free(B->array);
    free(B);
    
    return true;
}

bool insertElementOnBT(BT_t *B, int keyValue, void *element) {
    for (int i=0; i<B->capacity; i++) {
        if (B->array[i] == NULL) {
            BTN_t *node = malloc(sizeof(BTN_t));
            if (node == NULL) return false;
            node->keyValue = keyValue;
            node->element = element;
            B->array[i] = node;
            return true;
        }
    }
    return false;
}

bool deleteElementOnBT(BT_t *B, int keyValue) {
    //    Consider a subtree rooted at the node to be deleted.
    //    Replace the deletion node with the leftmost leaf of the subtree.
    int root = 0;
    if (B->array[root] < 0) return false;
    
    int target = findElementIndexOnBT(B, 0, keyValue, BT_OPTION_BREADTH_FIRST_SEARCH);
    if (target < 0) return false;
    int leftmost = findLeftmostLeefIndexOnBT(B, target);
    
    // delete node
    if (target == leftmost) {
        free(B->array[target]->element);
        free(B->array[target]);
        B->array[target] = NULL;
    }
    else {
        free(B->array[target]->element);
        free(B->array[target]);
        B->array[target] = B->array[leftmost];
        B->array[leftmost] = NULL;
    }
    
    return true;
}

void *findElementOnBT(BT_t *B, int keyValue, BT_OPTION_e option) {
    int index = findElementIndexOnBT(B, 0, keyValue, option);
    if (index >= 0) return B->array[index]->element;
    return NULL;
}

int levelOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter) {
    if (B == NULL) return -1;
    if (rootIndex < 0) return -1;
    
    for (int i=rootIndex; i<B->capacity; i++) {
        if (B->array[i] != NULL) {
            int index = func(B, i, parameter);
            if (index >= 0) return index;
        }
    }
    
    return -1;
}

int preOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter) {
    if (B == NULL) return -1;
    if (rootIndex < 0) return -1;
    if (rootIndex >= B->capacity) return -1;
    
    //    printf("pre-order traversal : array[%d] = %d\n", root, B->array[root]);
    int index = -1;
    index = func(B, rootIndex, parameter);
    if (index >= 0) return index;
    index = preOrderTraversalOnBT(B, getLeftIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    index = preOrderTraversalOnBT(B, getRightIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    
    return -1;
}

int inOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter) {
    if (B == NULL) return -1;
    if (rootIndex < 0) return -1;
    if (rootIndex >= B->capacity) return -1;
    
    int index = -1;
    index = inOrderTraversalOnBT(B, getLeftIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    //    printf("in-order traversal : array[%d] = %d\n", root, B->array[root]);
    index = func(B, rootIndex, parameter);
    if (index >= 0) return index;
    index = preOrderTraversalOnBT(B, getRightIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    
    return -1;
}

int postOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter) {
    if (B == NULL) return -1;
    if (rootIndex < 0) return -1;
    if (rootIndex >= B->capacity) return -1;
    
    int index = -1;
    index = postOrderTraversalOnBT(B, getLeftIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    index = postOrderTraversalOnBT(B, getRightIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    //    printf("post-order traversal : array[%d] = %d\n", root, B->array[root]);
    index = func(B, rootIndex, parameter);
    if (index >= 0) return index;
    
    return -1;
}

int getHeightBT(BT_t *B, int rootIndex) {
    int rightmost = -1;
    for (int i=B->capacity - 1; i>=0; i--) {
        if (B->array[i] != NULL) {
            rightmost = i;
            break;
        }
    }
    int height = 0;
    if (rightmost >= 0) {
        height = floor(log2(rightmost + 1));
    }
    
    return height;
}

//////////////////////////////////////////////////
//  private
int findElementIndexOnBT(BT_t *B, int rootIndex, int keyValue, BT_OPTION_e option) {
    int index = -1;
    switch (option) {
        case BT_OPTION_BREADTH_FIRST_SEARCH:
            //            return breadthFirstFindElementIndexOnBT(B, value);
            index = levelOrderTraversalOnBT(B, rootIndex, findElementIndexOnBTslave, &keyValue);
            if (index >= 0) return index;
            break;
        case BT_OPTION_DEPTH_FIRST_SEARCH:
            index = preOrderTraversalOnBT(B, rootIndex, findElementIndexOnBTslave, &keyValue);
            if (index >= 0) return index;
            break;
        default:
            break;
    }
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

int findElementIndexOnBTslave(BT_t *B, int nodeIndex, void *keyValue) {
    // Block illegal parameters.
    if (B == NULL) return -1;
    if (nodeIndex < 0) return -1;
    
    if (B->array[nodeIndex]->keyValue == *((int*)keyValue)) return nodeIndex;
    
    return -1;
}

//////////////////////////////////////////////////
//  debug
void viewBT(BT_t *B, BT_OPTION_e option) {
    int root = 0;
    int wordWidth = 4;
    if (option == BT_OPTION_VIEW_CHAR) {
        wordWidth = 2;
    }
    int height = getHeightBT(B, root);
    
    // veiw Binary Tree.
    int numOfNodes = (int)pow(2, height);
    int width = wordWidth * numOfNodes;
    char view[height+1][width+1];
    for (int i=0; i<=height; i++) {
        view[i][0] = '\0';
    }
    
    char format10[32] = "";
    char format11[32] = "";
    char format20[32] = "";
    char padding[32] = "";
    for (int i=0; i<wordWidth; i++) {
        strcat(padding, " ");
    }
    //    padding[wordWidth-1] = '_';
    sprintf(format10, "%%s%%%dd", wordWidth);
    sprintf(format11, "%%s%%%dc", wordWidth);
    sprintf(format20, "%%s%s", padding);
    
    int leftmostIndex = pow(2, height) - 1;
    int rightmostIndex = pow(2, height+1) - 2;
    for (int i=height; i>=0; i--) {
        int numOfPadding = (width - (wordWidth * numOfNodes))/numOfNodes;
        for (int j=0; (i!=height) && (j<numOfPadding/2); j++) {
            strcat(view[i], " ");
        }
        for (int j=leftmostIndex; j<=rightmostIndex; j++) {
            if ((j < B->capacity) &&
                (B->array[j] != NULL)) {
                if (option == BT_OPTION_VIEW_INT) {
                    sprintf(view[i], format10, view[i], B->array[j]->keyValue);
                }
                else {
                    sprintf(view[i], format11, view[i], (char)B->array[j]->keyValue);
                }
            }
            else {
                sprintf(view[i], format20, view[i]);
            }
            for (int k=0; (i!=height) && (k<numOfPadding); k++) {
                strcat(view[i], " ");
                if (strlen(view[i]) >= width - 1) break;
            }
        }
        leftmostIndex = getParent(leftmostIndex);
        rightmostIndex = getParent(rightmostIndex);
        numOfNodes = numOfNodes >> 1;
    }
    for (int i=0; i<=height; i++) {
        printf("\nLevel #%02d %s\n", i, view[i]);
    }
    printf("\n");
}
