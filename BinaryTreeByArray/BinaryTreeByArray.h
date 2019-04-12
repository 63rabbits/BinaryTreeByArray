#ifndef BinaryTreeByArray_h
#define BinaryTreeByArray_h

//////////////////////////////////////////////////
typedef enum BTOption {
    BT_OPTION_NONE,
    BT_OPTION_WITH_ELEMENT,

    BT_OPTION_BREADTH_FIRST_SEARCH = 10,
    BT_OPTION_DEPTH_FIRST_SEARCH,

    BT_OPTION_VIEW_INT = 20,
    BT_OPTION_VIEW_CHAR
} BT_OPTION_e;

typedef struct BinaryTree_Node {
    int keyValue;
    void *element;
} BTN_t;

typedef struct BinaryTree {
    int capacity;
    BTN_t **array;
} BT_t;

//////////////////////////////////////////////////
BT_t *createBT(int capacity);
bool destroyBT(BT_t *B, BT_OPTION_e option);
bool insertElementOnBT(BT_t *B, int keyValue, void *element);
bool deleteElementOnBT(BT_t *B, int keyValue);
void *findElementOnBT(BT_t *B, int keyValue, BT_OPTION_e option);
int levelOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter);
int preOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter);
int inOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter);
int postOrderTraversalOnBT(BT_t *B, int rootIndex, int (*func)(BT_t*, int, void*), void *parameter);
int getHeightBT(BT_t *B, int rootIndex);
// for debug
void viewBT(BT_t *B, BT_OPTION_e option);

#endif
