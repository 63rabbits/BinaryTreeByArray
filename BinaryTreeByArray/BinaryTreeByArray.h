#ifndef BinaryTreeByArray_h
#define BinaryTreeByArray_h

//////////////////////////////////////////////////
#define getParent(v) ((v - 1) / 2)
#define getLeftIndex(v) (v * 2 + 1)
#define getRightIndex(v) (v * 2 + 2)
#define max(a, b) (a > b ? a : b)

typedef enum BTOption {
    BT_OPTION_BREADTH_FIRST_SEARCH,
    BT_OPTION_DEPTH_FIRST_SEARCH
} BT_OPTION_e;

typedef struct BinaryTree {
    int capacity;
    int *array;
} BT_t;

//////////////////////////////////////////////////
BT_t *createBT(int capacity);
bool destroyNodeBT(BT_t *B);
bool insertElementOnBT(BT_t *B, int value);
bool deleteElementOnBT(BT_t *B, int value);
int findElementOnBT(BT_t *B, int value, BT_OPTION_e option);
int findElementIndexOnBT(BT_t *B, int value, BT_OPTION_e option);
int breadthFirstFindElementIndexOnBT(BT_t *B, int value);
int depthFirstFindElementIndexOnBT(BT_t *B, int root, int value);
int findLeftmostLeefIndexOnBT(BT_t *B, int root);
void levelOrderTraversalOnBT(BT_t *B, int root);
void preOrderTraversalOnBT(BT_t *B, int root);
void inOrderTraversalOnBT(BT_t *B, int root);
void postOrderTraversalOnBT(BT_t *B, int root);

#endif
