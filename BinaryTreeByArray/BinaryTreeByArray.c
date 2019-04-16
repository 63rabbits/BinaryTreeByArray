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
void autoExpandArrayOnBT(BT_t *B);
int findElementIndexOnBT(BT_t *B, int rootIndex, int keyValue, BT_OPTION_e option);
int findLeftmostLeefIndexOnBT(BT_t *B, int rootIndex);
int findElementIndexOnBTslave(BT_t *B, int rootIndex, void *keyValue);

//////////////////////////////////////////////////
//  public
BT_t *createBT(void) {
    BT_t *B = malloc(sizeof(BT_t));
    if (B == NULL) return NULL;
    B->capacity = BT_INITIAL_CAPACITY;
    B->array = calloc(B->capacity, sizeof(BTN_t**));
    if (B->array == NULL) {
        free(B);
        return NULL;
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
//    for (int i=0; i<B->capacity; i++) {
    int i = 0;
    while (true) {
        if (i >= B->capacity) {
            autoExpandArrayOnBT(B);
        }
        if (B->array[i] == NULL) {
            BTN_t *node = malloc(sizeof(BTN_t));
            if (node == NULL) return false;
            node->keyValue = keyValue;
            node->element = element;
            B->array[i] = node;
            return true;
        }
        i++;
    }
    return false;
}

bool deleteElementOnBT(BT_t *B, int keyValue) {
    //    Consider a subtree rooted at the node to be deleted.
    //    Replace the deletion node with the leftmost leaf of the subtree.
    int rootIndex = 0;
    if (B->array[rootIndex] == NULL) return false;
    
    int findIndex = findElementIndexOnBT(B, rootIndex, keyValue, BT_OPTION_BREADTH_FIRST_SEARCH);
    if (findIndex < 0) return false;
    int leftmostIndex = findLeftmostLeefIndexOnBT(B, findIndex);
    
    // delete node
    if (findIndex == leftmostIndex) {
        free(B->array[findIndex]->element);
        free(B->array[findIndex]);
        B->array[findIndex] = NULL;
    }
    else {
        free(B->array[findIndex]->element);
        free(B->array[findIndex]);
        B->array[findIndex] = B->array[leftmostIndex];
        B->array[leftmostIndex] = NULL;
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
    if (B->array[rootIndex] == NULL) return -1;
    
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
    if (B->array[rootIndex] == NULL) return -1;

    int index = -1;
    index = inOrderTraversalOnBT(B, getLeftIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
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
    if (B->array[rootIndex] == NULL) return -1;

    int index = -1;
    index = postOrderTraversalOnBT(B, getLeftIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    index = postOrderTraversalOnBT(B, getRightIndex(rootIndex), func, parameter);
    if (index >= 0) return index;
    index = func(B, rootIndex, parameter);
    if (index >= 0) return index;
    
    return -1;
}

int getHeightBT(BT_t *B, int rootIndex) {
    int righmostIndex = -1;
    for (int i=B->capacity - 1; i>=0; i--) {
        if (B->array[i] != NULL) {
            righmostIndex = i;
            break;
        }
    }
    int height = 0;
    if (righmostIndex >= 0) {
        height = floor(log2(righmostIndex + 1));
    }
    
    return height;
}

void *getElementOnBT(BT_t *B, int rootIndex) {
    // Block illegal parameters.
    if (B == NULL) return NULL;
    
    return B->array[rootIndex]->element;
}

//////////////////////////////////////////////////
//  private
void autoExpandArrayOnBT(BT_t *B) {
    int newSize = B->capacity << 1;
    B->array = realloc(B->array, sizeof(BTN_t**) * newSize);
    for (int i=B->capacity; i<newSize; i++) {
        B->array[i] = NULL;
    }
    B->capacity = newSize;
}

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

int findLeftmostLeefIndexOnBT(BT_t *B, int rootIndex) {
    if (rootIndex < 0) return -1;
    if (rootIndex >= B->capacity) return -1;
    
    int leftIndex = -1;
    int rightIndex = -1;
    leftIndex = findLeftmostLeefIndexOnBT(B, getLeftIndex(rootIndex));
    if ((leftIndex >= 0) &&
        (leftIndex < B->capacity) &&
        (B->array[leftIndex] == NULL)) {
        rightIndex = findLeftmostLeefIndexOnBT(B, getRightIndex(rootIndex));
    }
    return max(max(leftIndex, rightIndex), rootIndex);
}

int findElementIndexOnBTslave(BT_t *B, int rootIndex, void *keyValue) {
    // Block illegal parameters.
    if (B == NULL) return -1;
    if (rootIndex < 0) return -1;
    
    if (B->array[rootIndex]->keyValue == *((int*)keyValue)) return rootIndex;
    
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

