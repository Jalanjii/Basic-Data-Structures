#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;
 
 
//---------------List------------------
typedef struct List {
    int size;
    Node* head;
    Node* tail; 
} List;

const List list_init = { .size = 0, .head = NULL, .tail = NULL };

List* create_list() {
    List* list = malloc(sizeof * list);
    if (!list) return list;
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}
 
Node* create_node(List *l, int elm) {
    Node* node = malloc(sizeof * node);
    if (!node) return node;
    node->data = elm;
    node->next = NULL;
    l->size++;
    return node;
}
 
Node* get_tail(List *l) {
    return l->tail;
}
 
int is_empty_sll(List *l) {
    return l->head == NULL;
}
 
int size_sll(List *l) {
    return l->size;
}
 
Node* node_k_sll(List *l, size_t k) { // 0-based index
    if (!is_empty_sll(l) && (k >= 0 && k <= size_sll(l) -1))  {
        Node* node = l->head;
        while (k--)
            node = node->next;
        return node;
    }
    printf("Invalid k or List is Empty");
    return NULL;
}
 
void append_sll(List *l, int elm) { //O(1)
    Node* cur = create_node(l, elm);
    if (!l->head) {
        l->head = cur;
    }
    else {
        l->tail->next = cur;
    }
    l->tail = cur;
}
 
void prepend_sll(List *l, int elm) {
    Node* updated_head = create_node(l, elm);
    if (!l->head) {
        l->head = updated_head;
        l->tail = l->head;
    }
    else {
        updated_head->next = l->head;
        l->head = updated_head;
    }
}
 
void insert_sll(List *l, int elm, size_t i) {
    size_t n = size_sll(l);
    if (is_empty_sll(l)) {
        printf("List Is Empty");
    }
    else if (i == 0) {
        return prepend_sll(l, elm);
    }
    else if (i == n) {
        return append_sll(l, elm);
    }
    if (i > 0 && i < n) { 
        Node* cur = create_node(l, elm);
        Node* last = l->head;
        while (--i)
            last = last->next;
        cur->next = last->next;
        last->next = cur;
    }
    else 
        printf("Invalid i");
}
 
int delete_sll(List *l, int elm) {
    size_t n = size_sll(l), found = 0;
    Node* node = l->head;
    if (l->head->data == elm) {
        l->head = l->head->next;
        if (!l->head)
            l->tail = NULL;
        found = 1;
        free(node);
    }
    else {
        Node* prev = l->head;
        for (int i = 0; i < n; i++) {
            if (node->data != elm) {
                prev = node;
                node = node->next;
            }
            else {
                found = 1;
                break;
            }
        }
        prev->next = node->next;
        if (node->data == l->tail->data) //?
            l->tail = prev;
        free(node);
    }
    if (found) {
        l->size -= 1;
        return 1;
    }
    else
        return -1;
}
 
int find_sll(List *l, int elm) {// returns -1 if element not found
    int found = 0;
    if (!is_empty_sll(l)) {
        int i, n = size_sll(l);
        Node* last = l->head;
        for (i = 0; i < n; i++) {
            if (last->data != elm) {
                last = last->next;
            }
            else {
                found = 1;
                break;
            }
        }
        if (found) return i;
        else return -1;
    }
    else return -1;
}
 
void swap_val_sll(List *l, int i, int j) {// swap value of position i with j
    Node* node_i = node_k_sll(l, i);
    Node* node_j = node_k_sll(l, j);
    int temp = node_i->data;
    node_i->data = node_j->data;
    node_j->data = temp;
}

void swap_sll(Node* p, Node* q) {// swap value of position i with j
    int temp = p->data;
    p->data = q->data;
    q->data = temp;
}
 
void print_sll(List *l) {
    for(Node* trav = l->head; trav != NULL; trav = trav->next)
        printf("%d ", trav->data);    
    printf("\n");
}
 
void reverse_sll(List *l) {
    if (!is_empty_sll(l)) {
        Node* prev = l->head;
        Node* cur = l->head->next, * next_node = cur;
        while (next_node) {
            next_node = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next_node;
        }
        l->head->next = next_node;
        Node* temp = l->head;
        l->head = l->tail;
        l->tail = temp;
    }
}
 
void clear_sll(List *l) { //O(n)
    while (l->head) {
        Node* temp = l->head;
        l->head = l->head->next;
        free(temp);
        l->size -= 1;
    }
    l->tail = NULL; //if not, it stays dangling
    printf("List Cleared!\n");
}
 
void rotate_sll(List *l, int k) {
    if (!is_empty_sll(l) && (k < l->size && l->size > 1)) {
        int size = size_sll(l);
        Node* k_node = node_k_sll(l, k);
        l->tail->next = l->head;
        for (int i = 0; i < size; i++) {
            printf("%d ", k_node->data);
            k_node = k_node->next;
        }
        l->tail->next = NULL;
        printf("\n");
    }
    else 
        printf("List Is Empty or Invalid k");
}
 
void delete_front_sll(List *l) {
    if (!is_empty_sll(l))
        delete_sll(l, l->head->data);
    else
        printf("List is Empty!");
}

Node* prev_node(List* l, Node* i_node) { // generalised
    Node* node = l->head, *prev = NULL;
    while (node != i_node) {
        prev = node;
        node = node->next;
    }
    return prev;
}

void delete_back_sll(List *l) {
    if (!is_empty_sll(l)) {
        Node* node = l->tail;
        l->tail = prev_node(l, l->tail);
        if (l->tail) 
            l->tail->next = NULL;
         else
            l->head = NULL;
		l->size--;
        free(node);
    }
    else 
        printf("List is Empty!");
}

void delete_node(List *l, Node* node) {
    if (node == l->head)
        delete_front_sll(l);
    else if (node == l->tail)
        delete_back_sll(l);
    else {
        Node* prev = prev_node(l, node);
        prev->next = node->next;
        free(node);
    }
        
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void print_arr(int* arr, int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void bubble_sort(List *l) { // slow: 2
    int flag = 0;
    Node* last = l->tail;
    for (Node* trav = l->head; trav != last; trav = trav->next) {
        for (Node* prev = l->head; prev != last; prev = prev->next) {
            if (prev->data > prev->next->data) {
                swap_sll(prev, prev->next);
                flag = 1;
            }
        }
        // optimization
        // last = last->prev; 
        if (!flag)
            break;
    }
}

void selection_sort(List *l) { // slow: 1 
    int min_data = l->head->data;
    Node* min_node, *start = l->head;
    for (Node* trav = l->head; trav != NULL; trav = trav->next) {
        start = trav;
        min_data = start->data;
        for (Node* node = start; node != l->tail; node = node->next) { //l->tail instead of NULL so we don't compare the same 
        //position of itself
            if (node->data < min_data) {
                min_data = node->data;
                min_node = node;
            }
        }
        swap_sll(trav, min_node);
    }
}

void insertion_sort_1(List *l) { // slow: 3 --shifting -- sorted | unsorted division
    Node* trav = l->head; // the idea is that from left to right, we only shift per one element, left=sort,it suffices 
    //only from left tp find <
    while (trav) {
        Node* temp = trav;
        int i = 0, data = trav->data, flag = 0;
        for (Node* prev = l->head; prev != trav; prev = prev->next) {
            if (data <= prev->data) {// = is important!
                trav = trav->next;
                flag = 1;
                delete_node(l, temp);
                insert_sll(l, data, i);
                break; //the key to insertion sort! We do once the steps inside 'if'
            }
            i++; //for array, we swap i(trav) - i(prev) times, 
        }
        if (!flag)
            trav = trav->next;
    }
}

void insertion_sort_2(int* arr, int n) {
    int temp, j;
    for (int i = 1; i < n; i++) {
        temp = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > temp) {
            swap(&arr[j], &arr[j + 1]);
            j--;
        }
    }
}

void shell_sort(int *arr, int n) { // slow: 4 
    int k = 1;
    while (3*k + 1 <= n)
        k = 3*k + 1;
    
    for (int gap = k; gap > 0; gap /= 3) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j - gap >= 0 && arr[j - gap] > temp) {
                swap(&arr[j-gap], &arr[j]);
                j -= gap;
            }
        }
    }
}

int random_partition(int* arr, int start, int end) {
    srand(time(NULL));
    int random_pivot = rand() % (start - end) + start;
    swap(&arr[random_pivot], &arr[end]);
    int pivot = arr[end], pivot_index = start;
    for (int i = start; i < end; i++) {
        if (arr[i] <= pivot) {
            swap(&arr[pivot_index], &arr[i]);
            pivot_index++;
        }
    }
    swap(&arr[pivot_index], &arr[end]);
    return pivot_index;
}

void quick_sort(int* arr, int start, int end) {
    if (start < end)  {
        int pivot_index = random_partition(arr, start, end);
        quick_sort(arr, start, pivot_index - 1);
        quick_sort(arr, pivot_index + 1, end);
    }
}

// --------merge sort with arrays --------------
void merge_arr(int* arr, int* left, size_t n_left, int* right, size_t n_right) {
    int i = 0, j = 0, k = 0;
    while (i < n_left && j < n_right) {
        if (left[i] < right[j]) 
            arr[k++] = left[i++];
        else 
            arr[k++] = right[j++];
    }
    for (; i < n_left; i++) {
        arr[k++] = left[i];
    }
    for (; j < n_right; j++) {
        arr[k++] = right[j];
    }
}

void merge_sort_arr(int* arr, int n) { 
    if (n > 1) {
        int mid = n / 2;
        int* left = calloc(mid, sizeof * left), *right = calloc(n-mid, sizeof * right);
        for (int i = 0; i < mid; i++)
            left[i] = arr[i];
        for (int i = mid; i < n; i++)
            right[i - mid] = arr[i];
            
        merge_sort_arr(left, mid);
        merge_sort_arr(right, n - mid);
        merge_arr(arr, left, mid, right, n - mid);
    }
    //return; is implicit here
}

// --------merge sort with linked lists --------------

List* split_half(List* list) {
    List *right = create_list();
    int n = size_sll(list);
    if (n == 0) return right;
    Node* tail = node_k_sll(list, (n - 1) / 2);
    right->head = tail->next;
    right->tail = list->tail;
    right->size = n - (n + 1) / 2;

    list->tail = tail;
    list->size = (n + 1) / 2;
    tail->next = NULL;
    return right;
}

void merge(List* list, List* rightList) {
    Node *left = list->head,*right = rightList->head;
    list->size += rightList->size;
    if (right == NULL || left != NULL && left->data <= right->data) {
        if (left != NULL) left = left->next;
    } else {
        list->head = right;
        right = right->next;
    }
    Node *tail = list->head;
    while (left != NULL && right != NULL) {
        if (left->data <= right->data) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next; 
    }
    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
        list->tail = rightList->tail;
    }
    // Empty the right list
    rightList->head = rightList->tail = NULL;
    rightList->size = 0;
}

void merge_sort(List* list) {
    if (list->head != NULL && list->head->next != NULL) {
        List *right = split_half(list);
        merge_sort(list);
        merge_sort(right);
        merge(list, right);
    }
}

// --------------- Problems ---------------

// problem 1: reverse a linked list using recursion
// solution:
Node* reverse_sll_r(List* list, Node* prev) {
    if (prev->next == NULL) {
        list->head = prev;
        return prev;
    }
    Node* cur = reverse_sll_r(list, prev->next);
    cur->next = prev;
    list->tail = prev;
    list->tail->next = NULL;
}

// problem 3: //https://leetcode.com/problems/linked-list-cycle/
// we know that if a cycle: 1- starting with fast - slow = 2, then with the cycle
// we will have fast < slow, and let s say the fast - slow = gap
// to close gap = 10, move slow then gap = gap +1 and move fast then gap = gap +1 -2 = gap - 1
// this means that given gap = g, the gap will be closed in O(g) where g < n, so the algorithm works in O(n)
// g = n - 1 is possible when fast - slow = 1
int has_cycle(List* list) { // Floyd's Tortoise and Hare detection beginning of cycle algorithm
    Node* slow = list->head, *fast = list->head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return 1;
    }
    return 0;
}

// problem 4: https://leetcode.com/problems/find-the-duplicate-number/
// version 1: modifying array - using array elements as index array and marking elements visited// 
int find_duplicate_1(int* arr, int n) { // O(n), O(1)
    for (int i = 0; i < n; i++) {
        int elm = arr[i];
        int idx = abs(elm);
        if (arr[idx] < 0) {
            return idx;
        }
        arr[idx] = -arr[idx];
    }
    return n; // not duplicate
}
// version 2:  without modifying the array - using Floyd's Tortoise and Hare detection cycle algorithm
// input values as indices or pointers, and we begin by index 0 and its value as pointer, so 0 is not included in the rest of array, it's just the head
// and thus, it's not gonna be part of any cycle, as explained in the problem statement
int find_duplicate_2(int* arr) { // O(n), O(1)
    int slow = arr[0];
    int fast = arr[arr[0]];
    while (slow != fast) { // part 1: floyd's end of cycle
        slow = arr[slow];
        fast = arr[arr[fast]];
    }
    slow = 0;
    while (slow != fast) { // part 2: beg of cycle: see image find_duplicate_2 (D(arr[0], beg)) = D(beg, end)
        slow = arr[slow];
        fast = arr[fast];
    }
    return slow;
}

int main() {
    // #define MAX 6
    // int n = MAX;
    // List l1 = list_init;
    // int arr[MAX] = {4, 5, 6, 1, 2, 3};
    // if (!arr) return 0;
    // for (int i = 0; i < n; i++)
    // 	append_sll(&l1, arr[i]);
		
    // print_sll(&l1);
    // reverse_sll_r(&l1, l1.head);
    // bubble_sort(&l1);
    // print_sll(&l1);
    // selection_sort(&l1);
    // print_sll(&l1);
    // insertion_sort_1(&l1);
    // print_sll(&l1);
    // insertion_sort_2(arr, n);
    // print_arr(arr, n);
    // shell_sort(arr, n);
    // print_arr(arr, n);
    // quick_sort(arr, 0, n-1);
    // print_arr(arr, n);
    // merge_sort_arr(arr, n);
    // print_arr(arr, n);
    // merge_sort(&l1);
    // print_sll(&l1);
    // clear_sll(&l1);

    
    // problems
    int n = 5;
    int arr[5] = {1, 2, 1, 3, 4};
    printf("duplicate: %d ", find_duplicate_2(arr));
    return 0;
}























