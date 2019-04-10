#include <unistd.h>
//#define checkMemoryLeak

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "BinaryTreeByArray.h"

//////////////////////////////////////////////////
typedef struct Element {
    int value;
} Element_t;

//////////////////////////////////////////////////
void test(void);
Element_t *createElement(int value);

//////////////////////////////////////////////////
int main(int argc, const char * argv[]) {
    
#ifdef checkMemoryLeak
    while (true) {
        test();
        sleep(1);
    }
#else
    test();
#endif
    
    return EXIT_SUCCESS;
}

void test() {
    int capacity = 7;
    int findValue = 5;
    int deleteValue = 2;
    const int root = 0;
    
    BT_t *binaryTree = createBT(capacity);
    for (int i=0; i<capacity; i++) {
        insertElementOnBT(binaryTree, i);
    }
    
    printf("*** level-order traversal ***\n");
    levelOrderTraversalOnBT(binaryTree, root);
    
    printf("*** pre-order traversal ***\n");
    preOrderTraversalOnBT(binaryTree, root);
    
    printf("*** in-order traversal ***\n");
    inOrderTraversalOnBT(binaryTree, root);
    
    printf("*** post-order traversal ***\n");
    postOrderTraversalOnBT(binaryTree, root);
    
    printf("*** breadth first find ***\n");
    int i = findElementIndexOnBT(binaryTree, findValue, BT_OPTION_BREADTH_FIRST_SEARCH);
    int v = findElementOnBT(binaryTree, findValue, BT_OPTION_BREADTH_FIRST_SEARCH);
    printf("breadth first find [%s] : index = %D, value = %d\n", __func__, i, v);
    
    printf("*** depth first find ***\n");
    i = findElementIndexOnBT(binaryTree, findValue, BT_OPTION_DEPTH_FIRST_SEARCH);
    v = findElementOnBT(binaryTree, findValue, BT_OPTION_DEPTH_FIRST_SEARCH);
    printf("depth first find [%s] : index = %D, value = %d\n", __func__, i, v);
    
    printf("*** delete ***\n");
    bool c = deleteElementOnBT(binaryTree, deleteValue);
    if (c) {
        printf("delete value %d : success.\n", deleteValue);
    }
    else {
        printf("error [%s] : could not delete value = %d.\n", __func__, deleteValue);
    }
    printf("*** level-order traversal ***\n");
    levelOrderTraversalOnBT(binaryTree, root);
    
    destroyNodeBT(binaryTree);
}

Element_t *createElement(int value) {
    Element_t *e = malloc(sizeof(Element_t));
    if (e == NULL) return NULL;
    e->value = value;
    return e;
}
