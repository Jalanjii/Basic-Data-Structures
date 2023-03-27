#include <stdio.h>
#include <stdlib.h>
 
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;
 
 
//---------------List------------------
typedef struct List {
    int size;
    Node* head;
    Node* tail; 
} List;
 
const List list_init = { .size = 0, .head = NULL, .tail = NULL };
 
Node* create_node(List *l, int elm) {
    Node* node = malloc(sizeof * node);
    if (!node) return node;
    node->data = elm;
    node->prev = NULL;
    node->next = NULL;
    l->size++;
    return node;
}
 
Node* get_tail(List *l) {
    return l->tail;
}
 
int is_empty_dll(List *l) {
    return l->head == NULL;
}
 
int size_dll(List *l) {
    return l->size;
}
 
Node* node_k_dll(List *l, size_t k) { // 0-based index
    if (!is_empty_sll(l) && (k >= 0 && k <= size_sll(l) -1))  {
        Node* node = l->head;
        while (k--)
            node = node->next;
        return node;
    }
    printf("Invalid k or List is Empty");
    return NULL;
}
 
void append_dll(List *l, int elm) { //O(1)
    Node* cur = create_node(l, elm);
    if (!l->head) {
        l->head = cur;
    }
    else {
        l->tail->next = cur;
        cur->prev = l->tail;
    }
    l->tail = cur;
}
 
void prepend_dll(List *l, int elm) {
    Node* updated_head = create_node(l, elm);
    if (!l->head) {
        l->head = updated_head;
        l->tail = l->head;
    }
    else {
        updated_head->next = l->head;
        l->head->prev = updated_head;
        l->head = updated_head;
    }
}
 
void insert_dll(List *l, int elm, size_t i) {
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
        (last->next)->prev = cur;
        cur->prev = last;
        last->next = cur;
    }
    else printf("Invalid i");
}
 
int delete_dll(List *l, int elm) {
    size_t n = size_sll(l), found = 0;
    Node* node = l->head;
    if (l->head->data == elm) {
        l->head = l->head->next;
        if (l->head) l->head->prev = NULL;
        else l->tail = NULL;
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
        if (node->data != l->tail->data)
            (node->next)->prev = prev;
        else 
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
 
int find_dll(List *l, int elm) {// returns -1 if element not found
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
 
void swap_dll(List *l, int i, int j) {// swap value of position i with j
    Node* node_i = node_k_sll(l, i);
    Node* node_j = node_k_sll(l, j);
    int temp = node_i->data;
    node_i->data = node_j->data;
    node_j->data = temp;
}
 
void print_dll(List *l, char p) {
    if (p == 'f') {
        for(Node* trav = l->head; trav != NULL; trav = trav->next)
            printf("%d ", trav->data);    
    }
    else {
        for(Node* trav = l->head; trav != NULL; trav = trav->next)
            printf("%d ", trav->data);    
    }
    printf("\n");
}
 
void reverse_dll(List *l) {
    if (!is_empty_sll(l)) {
        Node* node = l->head, *temp, *prev;
        while (node) {
            prev = node;
            node = node->next;
            temp = prev->next;
            prev->next = prev->prev;
            prev->prev = temp;  
        }
        temp = l->head;
        l->head = l->tail;
        l->tail = temp;
    }
}
 
void clear_dll(List *l) { //O(n)
    while (l->head) {
        Node* temp = l->head;
        l->head = l->head->next;
        free(temp);
        l->size -= 1;
    }
    l->tail = NULL; //if not, it stays dangling
    printf("List Cleared!\n");
}
 
void rotate_dll(List *l, int k, char p) {
    if (!is_empty_sll(l) && (k < l->size && l->size > 1)) {
        int size = size_sll(l);
        Node* k_node = node_k_sll(l, k);
        l->head->prev = l->tail;
        l->tail->next = l->head;
        if (p == 'f') {
            for (int i = 0; i < size; i++) {
                printf("%d ", k_node->data);
                k_node = k_node->next;
            }
        }
        else {
            for (int i = 0; i < size; i++) {
                printf("%d ", k_node->data);
                k_node = k_node->prev;
            }
        }
        l->tail->next = NULL;
        l->head->prev = NULL;
        printf("\n");
    }
    else printf("List Is Empty or Invalid k");
}
 
void delete_front_dll(List *l) {
    if (!is_empty_sll(l))
        delete_sll(l, l->head->data);
    else
        printf("List is Empty!");
}
 
void delete_back_dll(List *l) {
    if (!is_empty_sll(l)) {
        Node* node = l->tail;
        l->tail = node->prev;
        if (l->tail) 
            l->tail->next = NULL;
         else
            l->head = NULL;
		l->size--;
        free(node);
    }
    else printf("List is Empty!");
}
 
// void sort_sll() {//
 
 
int main() {
    
    List l1 = list_init;
    
    prepend_dll(&l1, 10);
    append_dll(&l1, 1);
    append_dll(&l1, 2);
    append_dll(&l1, 4);
    insert_dll(&l1, 3, 3);
    print_dll(&l1, 'f');
    reverse_dll(&l1);
    print_dll(&l1, 'f');
    rotate_dll(&l1, 2, 'f');
    insert_dll(&l1, 3, 3);
    print_dll(&l1, 'f');
    prepend_dll(&l1, 0);
    print_dll(&l1, 'f');
    printf("size: %d\n", size_sll(&l1));
    printf("index of 3 is: %d\n", find_sll(&l1, 3));
    delete_dll(&l1, 2);
    print_dll(&l1, 'f');
    swap_dll(&l1, 0, 1);
    print_dll(&l1, 'f');
    reverse_dll(&l1);
    print_dll(&l1, 'f');
    append_dll(&l1, 0);
    append_dll(&l1, -1);
    print_dll(&l1, 'f');
    rotate_dll(&l1, 2, 'f');
    delete_front_dll(&l1);
    print_dll(&l1, 'f');
    delete_back_dll(&l1);
    print_dll(&l1, 'f');
    append_dll(&l1, 20);
    prepend_dll(&l1, -20);
    print_dll(&l1, 'f');
    clear_dll(&l1);
 
    return 0;
}