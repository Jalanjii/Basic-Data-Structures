#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

//---------------------Stack---------------------

typedef struct Stack {
    int size;
    int min;
    Node* head;
    Node* tail;
} Stack;

const Stack stack_init = { .size = 0, .min = 0, .head = NULL, .tail = NULL };

Node* create_node(int elm) {
    Node* node = malloc(sizeof * node);
    if (!node) return node;
    node->data = elm;
    node->next = NULL;
    return node;
}

Stack* create_stack() {
    Stack* s = malloc(sizeof * s);
    s->size = 0;
    s->min = 0;
    s->head = NULL;
    s->tail = NULL;
    return s;
}

int is_empty_s(Stack *s) {
    return s->tail == NULL;
}

void push(Stack *s, int elm) {
    Node* updated_head = create_node(elm);
    if (!s->head) {
        s->min = elm;
        s->head = updated_head;
        s->tail = s->head;
    } else {
        if (updated_head->data < s->min) {
            // 2* newMin-prevMin <= newMin, so it works as a flag to check for keys that were chcked previous as minimum in the stacl
            // and as you can see, at each minimum location, we have access to the prev minimum with the help of current s->min
            updated_head->data = 2*elm - s->min;
            s->min = elm;
        }
        updated_head->next = s->head;
        s->head = updated_head;
    }
    s->size++;
}

int pop(Stack *s) {
    if (!is_empty_s(s)) {
        Node* node = s->head;
        int elm = node->data;
        s->head = s->head->next;
        if (elm < s->min) {
            s->min = 2*s->min - elm;
            int original = (s->min + elm) / 2;
            elm = original;
        }
        if (!s->head)
            s->tail = NULL;
        s->size--;
        free(node);
        if (is_empty_s(s)) s->min = -111;
        return elm;
    }
    return -111; //assuming it -111 won't be in the stack
    
}

int get_min(Stack* s) {
    return s->min;
}

int top(Stack *s) {
    int top;
    if (s->head)
        top = s->head->data;
    else
        top = -111;
    return top;
}

void clear_s(Stack *s) {
    while (s->tail)
        pop(s);
}

Stack* reverse_it(Stack *s) { //iterative
    Stack *s2 = malloc(sizeof *s2);
    if (!s2) return s2;
    *s2 = stack_init;

    while (s->tail)
        push(s2, pop(s));
    s->head = s2->head;
    return s;
}

void print_s(Stack *s) { // this deals with the min case altering the original array, so we have to pop and push again
    if (!is_empty_s(s)) {
        int data = pop(s);
        // or place here instead printf("%d ", data); if you want to print the reverse of the stack
        print_s(s);
        printf("%d ", data);
        push(s, data);
    }
}

// ----------------Probelems & Solutions ------------------
// problem1: reverse a stack using recursion
void insert_bottom(Stack *s, int elm) {
    if (is_empty_s(s)) {
        push(s, elm);
    }
    else {
        int top = pop(s);
        insert_bottom(s, elm);
        push(s, top);
    }
}

Stack* reverse_re(Stack *s) { //recursive
    if (!is_empty_s(s)) {
        int top = pop(s);
        reverse_re(s);
        insert_bottom(s, top);
    }
    return s;
}

// problem2: https://leetcode.com/problems/valid-parentheses/
int is_valid(Stack* s, char* string, size_t n) { // balanced: closed before opening of other brackets
    for (int i = 0; i < n; i++) {
        char elm = string[i];
        char tops = top(s);
        if (elm == '{' || elm == '(' || elm == '[')
            push(s, elm);
        else if (tops == '(' && elm == ')' || tops == '{' && elm == '}' || tops == '[' && elm == ']')
            pop(s);
    }
    if (is_empty_s(s))
        return 1;
    return 0;
}

// problem3: https://leetcode.com/problems/min-stack/
// -- designed above efficiently : https://www.youtube.com/watch?v=QMlDCR9xyd8

// problem4: https://leetcode.com/problems/daily-temperatures/ [monotonic stack]
int* daily_temp(int* temp, const int n) {
    Stack* s; // we will store indices instead of a pair of indices and values, the value we will get by access to the original array
    int* res = calloc(n, sizeof * res);
    for (int i = 0; i < n; i++) {
        while (!is_empty_s(s) && temp[i] > temp[top(s)]) {
            res[top(s)] = i - top(s);
            pop(s);
        }
        push(s, i);
    }
    return res;
}

void print_arr(int* arr, int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
}

int main() {
    
    int n = 8;
    // char string[16] = {'[', '(', ')',']','{','}','{','[','(',')','(',')',']','(',')','}'}; 
    int temp[8] = {73, 74, 75, 71, 69, 72, 76, 73};
    Stack s1 = stack_init;
    push(&s1, 5);
    push(&s1, 4);
    push(&s1, 3);
    push(&s1, 2);
    push(&s1, 1);
    pop(&s1);
    pop(&s1);
    pop(&s1);
    pop(&s1);
    pop(&s1);
    // print_s(&s1);
    // reverse_it(&s1);
    // reverse_re(&s1);
    // print_s(&s1);
    // printf("string [()]{}{[()()]()} is %d \n", is_valid(&s1, string, n));
    // printf("min is %d \n", get_min(&s1));
    print_arr(daily_temp(temp, n), n);
    
    
    return 0;
}

