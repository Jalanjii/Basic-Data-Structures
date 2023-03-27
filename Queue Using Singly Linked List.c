#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;


typedef struct Queue {
    int size;
    Node* head;
    Node* tail;
} Queue;

const Queue queue_init = { .size = 0, .head = NULL, .tail = NULL };

int is_empty_q(Queue *q) {
    return q->tail == NULL;
}

Node* create_node(int elm) {
    Node* node = malloc(sizeof * node);
    if (!node) return node;
    node->data = elm;
    node->next = NULL;
    return node;
}

Node* tail_prev(Queue *q) {
    Node* node = q->head, *prev = NULL;
    while (node->next) {
        prev = node;
        node = node->next;
    }
    return prev;
}

void enqueue(Queue *q, int elm) {
    Node* updated_head = create_node(elm);
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

int dequeue(Queue *q) {
    if (!is_empty_q(q)) {
        Node* node = q->tail;
        int elm = q->tail->data;
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
    return -111; //assuming it -111 won't be in the queue
}

int front(Queue *q) {
    int front;
    if (q->tail)
        front = q->tail->data;
    else
        front = -111;
    return front;
}

void clear_q(Queue *q) {
    while (q->tail)
        dequeue(q);
    printf("Queue Cleared");
}

Queue* reverse_q(Queue *q) { //iterative
    Queue *q2 = malloc(sizeof *q2);
    if (!q2) return q2;
    *q2 = queue_init;
    size_t size_e = q->size, size = q->size;
    for (size_t i = 0; i < size_e; i++) {
        while (size-- - 1)
            enqueue(q, dequeue(q));
        enqueue(q2, dequeue(q));
        size = q->size;
    }
    q->head = q2->head;
    return q;
}

void print_q(Queue *q) {
    for(Node* trav = q->head; trav != NULL; trav = trav->next)
        printf("%d ", trav->data);    
    printf("\n");
}


int main() {

    Queue q1 = queue_init;
    enqueue(&q1, 5);
    enqueue(&q1, 4);
    print_q(&q1);
    reverse_q(&q1);
    print_q(&q1);
    return 0;
}



