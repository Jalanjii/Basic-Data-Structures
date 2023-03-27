#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// n nodes -> n-1 edges/links -> depth(x) = n edges -> height(x)= longest path from x to a leaf
// max(debth(tree)) = height(tree); max number of nodes at level i = 2^int
//max number of nodes n in a tree with height h is n = 2^(h+1) -1 -> h = log_2(n+1) -1/ h+1 is num of levels
//height of complete bin tree is h = floor(log_2(n)) = min_height, whereas max_height = n-1
//diff = |h_left - h_right|
//height(emptyTree) = -1, height(tree with one node) = 0
//balanced means height_left - height_right <= -1
//insert and delete distort the balance

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

//---------------Queue------------------
typedef struct QNode {
    Node* data;
    struct QNode* next;
} QNode;

typedef struct Queue {
    int size;
    QNode* head;
    QNode* tail;
} Queue;

const Queue queue_init = { .size = 0, .head = NULL, .tail = NULL };

QNode* create_node(Node* elm) {
    QNode* node = malloc(sizeof * node);
    if (!node) return node;
    node->data = elm;
    node->next = NULL;
    return node;
}

int is_empty_q(Queue *q) {
    return q->tail == NULL;
}

QNode* tail_prev(Queue *q) {
    QNode* node = q->head, *prev = NULL;
    while (node->next) {
        prev = node;
        node = node->next;
    }
    return prev;
}

void enqueue(Queue *q, Node* elm) {
    QNode* updated_head = create_node(elm);
    if (!q->head) {
        q->head = updated_head;
        q->tail = q->head;
    }
    else {
        updated_head->next = q->head;
        q->head = updated_head;
    }
    q->size++;
}

Node* dequeue(Queue *q) {
    if (!is_empty_q(q)) {
        QNode* node = q->tail;
        Node* elm = q->tail->data;
        q->tail = tail_prev(q);
        if (q->tail) {
            q->tail->next = NULL;
        }
        else {
            q->head = NULL;
        }
        free(node);
        q->size--;
        return elm;
    }
    return NULL; //assuming it -111 won't be in the queue
}

Node* front(Queue *q) {
    Node* front;
    if (q->tail)
        front = q->tail->data;
    else
        front = NULL;
    return front;
}

void clear_q(Queue *q) {
    while (q->tail)
        dequeue(q);
    printf("Queue Cleared");
}

//---------------BST------------------
typedef struct BST { // search, store and update efficiently in O(log(n))
    Node* root;
} BST;
 
const BST bst_init = { .root = NULL };

int max(int x, int y) {
    return ((x > y) ? (x) : (y));
}

int min(int x, int y) {
    return ((x <= y) ? (x) : (y));
}

Node* create_node_bst(int elm) {
    Node* node = malloc(sizeof * node);
    if (!node) return node;
    node->data = elm;
    node->left = node->right = NULL;
    return node;
}

BST* create_bst() {
    BST* bst = malloc(sizeof * bst);
    if (!bst) return bst;
    bst->root = NULL;
    return bst;
}

int is_empty(Node* root) {
    return root == NULL;
}

Node* insert(Node* root, int elm) { // -V
    if (!root) {
        root = create_node_bst(elm);
    }
    else if (elm <= root->data) {
        root->left = insert(root->left, elm);
    }
    else {
        root->right = insert(root->right, elm);
    }
    return root;
}

Node* find(Node* root, int elm) { // -V
    if (!is_empty(root)) {
        if (!root) {
            root = NULL;
        }
        else if (root->data == elm) {
            root = root;
        }
        else if (elm <= root->data) {
            root = find(root->left, elm);
        }
        else {
            root = find(root->right, elm);
        }
        return root;
    }
    else
        return root;
}

Node* find_max(Node* root) { //-V
    if (!is_empty(root)) {
        if (root->right == NULL)
            return root;
        else {
            return find_max(root->right);
        }
    }
    else
        return root;
}

Node* find_min(Node* root) { //-V
    if (!is_empty(root)) {
        if (!root->left)
            return root;
        else {
            return find_min(root->left);
        }
    }
    else
        return root;
}

int height(Node* root) { // -V
    if (root == NULL) {
        return -1; // 0 if heighe is number of edges, or -1 if height=number of edges
    }
    int left_height = height(root->left);
    int right_height = height(root->right);
    return max(left_height, right_height) + 1;
}

int depth(Node* root, int elm) {
    if (root->data == elm) {
        return 0;
    }
    else if (elm < root->data) {
        return depth(root->left, elm) + 1;
    }
    else {
        return depth(root->right, elm) + 1;
    }
}
// case 1: no children, case 2: one chid, case 3: two children -> we choose the min of right subtree
//exploiting the property that a min node in a subtree doesn't have a left child
//so: we bring minimum from subtree so that the left is less and the right is either equal or greater
Node* delete(Node* root, int elm) { // -V
    if (root == NULL)
        return root;
    else if (elm > root->data)
        root->right = delete(root->right, elm);
    else if (elm < root->data)
        root->left = delete(root->left, elm);
    else { // elm found
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }
        else if (root->left == NULL) {
            Node* temp = root;
            root = root->right;
            free(temp);            
        }
        else if (root->right == NULL) {
            Node* temp = root;
            root = root->left;
            free(temp);
        }
        else { //this case is done until it is reduced to one of the previous three cases
            Node* temp = find_min(root->right);
            root->data = temp->data;
            root->right = delete(root->right, elm);
        }
    }
    return root;
}

int is_bst(Node* root, int min, int max) { // solution 1
    if (root == NULL) {
        return 1;
    }
    else if (root->data < max && root->data > min && is_bst(root->left, min, root->data) && is_bst(root->right, root->data, max)) 
        return 1;
    else
        return 0; 
} // solution 2, traverse inorder and check if the list is sorted

int is_bst_balanced(Node* root) {
    int is_balanced = 1;
    int left_height = height(root->left);
    int right_height = height(root->right);
    if (abs(right_height - left_height) > 1)
        is_balanced = 0;
    return is_balanced;
}

int size(Node* root) {
    if (!root)
        return 0;
    int left_size = size(root->left);
    int right_size = size(root->right);
    return left_size + right_size + 1; // + 1 is for the ancesstor
}

void level_order(Node* root) { // visit all children before grand children [BFS]
    if (!is_empty(root)) {
        Queue *q = malloc(sizeof *q);
        if (q) {
            *q = queue_init;
            enqueue(q, root);
            while (!is_empty_q(q)) {
                Node* cur = front(q);
                printf("%d ", cur->data);
                if (cur->left != NULL)
                    enqueue(q, cur->left);
                if (cur->right != NULL)
                    enqueue(q, cur->right);
                dequeue(q);
            }
        }
        free(q);
    }
}

void pre_order(Node* root) { //D<root>L<left>R<right> -- preorder (of root)
    if (root) {
        printf("%d ", root->data); // we can return an array stroing the order for other uses
        pre_order(root->left);
        pre_order(root->right);
    }
}

void in_order(Node* root) { //L<left>D<root>R<right> -- inorder -- gives sorted list
    if (root) {
        in_order(root->left);
        printf("%d ", root->data);
        in_order(root->right);
    }
}

Node* in_order_suc(Node* root, int data) { 
// case 1: node A has right sbtree: inoorder suc of a right subtree of node A (LDR) is the left most node in that subtree or min value 
// case 2: node A has no right sbtree: the nearst parent for which node A is on the left of that parent
    Node* cur = find(root, data);
    if (!cur) 
        return cur;
    if (cur->right != NULL) { //case 1: node has sub tree
        return find_min(cur->right);
    }
    else { //case 2: no right sub tree
        Node* suc = NULL, *prev = root;
        while (prev != cur) {
            if (cur->data < prev->data) {
                suc = prev;
                prev = prev->left;
            }
            else {
                prev = prev->right;
            }
        }
        return suc;
    }
}

void post_order(Node* root) { //L<left>R<right>R<root> -- postorder
    if (root) {
        post_order(root->left);
        post_order(root->right);
        printf("%d ", root->data);
    }
}

Node* clear(Node* root) {
    while (root) {
        root = delete(root, root->data);
    }
    return root;
}

// problem 1: https://leetcode.com/problems/invert-binary-tree/

void swap_right_left(Node* root) {
    Node* temp = root->left;
    root->left = root->right;
    root->right = temp;
}

void invert_bst(Node* root) {
    if (root) {
        swap_right_left(root);
        invert_bst(root->left);
        invert_bst(root->right);
    }
}

// problem 2: https://leetcode.com/problems/same-tree/
int compare_bst(Node* root_p, Node* root_q) {
    if (root_p && root_q) {
        if (root_p->data != root_q->data)
            return 0;
        return compare_bst(root_p->left, root_q->left) && compare_bst(root_p->right, root_q->right);
    }
    return !root_p && !root_q;
}

// problem 3: https://leetcode.com/problems/subtree-of-another-tree/
int has_subtree(Node* root, Node* sub_root) {
    if (root){
        if (compare_bst(root, sub_root))
            return 1;
        return has_subtree(root->left, sub_root) || has_subtree(root->right, sub_root);
    }
    return 0;
}

// problem 4: // https://leetcode.com/problems/count-good-nodes-in-binary-tree/
int good_nodes(Node* root) { 
    
}

// problem 5: // https://leetcode.com/problems/binary-tree-right-side-view/
Node* right_view(Node* root, int* res, int* i) { 
    if (root) {
        res[(*i)++] = root->data;
        root = right_view(root->right, res, i);
        if (!root->right) {
            return root;
        }
        if (root->left){
            right_view(root->left, res, i);
        }
        else if (!root->left) {
            return root;
        }
        return root;
    }
}
// problem 6: // https://leetcode.com/problems/kth-smallest-element-in-a-bst/
// version 1: easy O(1) space
// void in_order_k(Node* root, int* res, int* k) { //L<left>D<root>R<right> -- inorder -- gives sorted list
//     if (root) {
//         in_order_k(root->left, res, k);
//         if (!--(*k))
//             *res = root->data;
//         in_order_k(root->right, res, k);
//     }
// }

// int kth_smallest(Node* root, int k) {
//     int res = 0;
//     in_order_k(root, &res, &k);
//     return res;
// }
// version 2: tricky O(1) space
// the idea here is very simple, it is easily compared to the void signature. We simply follow an order, the order is to go deep left then go to root,
// so when we go left until we fun root = null, then that call with null returns and we go back one step to get that non null deep left to print its data, 
// the same logic we did in void we did in if(left != -1)
// the other most important idea, we only assigned to left as it is the most important to us in this order, the rest are just calls
// sometimes it is nice to see the logic of code in root and code outside of it separate to check 
// logic: with order related problems, as there are > 1 recursive call, always look for which for which recursive calls have the precedent like inorder
// is the left, and assign to variable to check, then do the logic below, like the two ifs, and then call other recursive calls 
int in_order_k(Node* root, int* k) { //L<left>D<root>R<right> -- inorder -- gives sorted list
    if (root) {
        int left = in_order_k(root->left, k);
        if (left != -1) // this will always return -1, unless we stop by the if (!--(*k)) to return, if not this latter if, the result will be -1
            return left;
        if (!--(*k)) // if not this
            return root->data;
        return in_order_k(root->right, k); // this will run till -1 and return that one, and that s the final result
    }
    return -1; // assuming -1 only marks if k > size of bst
}

int kth_smallest(Node* root, int k) {
    int res = in_order_k(root, &k);
    return res;
}

void print_arr(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    #define MAX 7
    int n = MAX;
    BST bst1 = bst_init;
    BST bst2 = bst_init;
    Node* bst1_root = bst1.root;
    Node* bst2_root = bst2.root;
    int arr[MAX] = {4, 1, 2, 3, 6, 7, 9};
	for (int i = 0; i < n; i++) {
	    bst1_root = insert(bst1_root, arr[i]);
	   // bst2_root = insert(bst2_root, arr[i]);
	   // if (i > 4) {
	   //     bst2_root = insert(bst2_root, arr[i]);
	   // }
	}
	
    int res[100], i = 0;
    right_view(bst1_root, res, &i);
    print_arr(res, i);
    // printf("kth_smallest for k = 3 is: %d \n", kth_smallest(bst1_root, 7));
    // printf("bst1 and bst2 are the same: %d \n", compare_bst(bst1_root, bst2_root));
    // printf("bst1_root has bst2 subtree: %d", has_subtree(bst1_root, bst2_root));
    // invert_bst(bst1_root);
    // printf("level order ");
    // level_order(bst1_root);
    // printf("\n");
    // printf("height: %d \n", height(bst1_root));
    // printf("size: %d \n", size(bst1_root));
    // printf("depth of 13: %d \n", depth(bst1_root, 18));
    // printf("is_bst: %d \n", is_bst(bst1_root, -1000, 1000)); //assuming -1000 < data(bst) < 1000
    // printf("is_bst_balanced: %d \n", is_bst_balanced(bst1_root));
    // printf("min: %d \n", find_min(bst1_root)->data);
    // printf("max: %d \n", find_max(bst1_root)->data);
    // printf("element: %d found \n", find(bst1_root, 19)->data);
    // printf("level order ");
    // level_order(bst1_root);
    // printf("\n");
    // printf("preorder order ");
    // pre_order(bst1_root);
    // printf("\n");
    // printf("inorder order ");
    // in_order(bst1_root);
    // printf("\n");
    // printf("postorder order ");
    // post_order(bst1_root);
    // printf("\n");
    // printf("inorder successor of 9 is %d \n", in_order_suc(bst1_root, 9)->data);
    // bst1_root = delete(bst1_root, 18);
    // printf("in order ");
    // in_order(bst1_root);
    // printf("\n");
    // bst1_root = insert(bst1_root, 18);
    // printf("in order ");
    // in_order(bst1_root);
    // bst1_root = clear(bst1_root);
    // printf("\n");
    if (!bst1_root)
        printf("BST Cleared!");
    return 0;
}

























