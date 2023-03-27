#include <stdio.h> // need review from the last review on codereview
#include <stdlib.h>
#include <math.h>
#include <time.h>

//full bt = max(nodes) of a given height, so if h = 3, then max(nodes) = 2^(3+1) -1 = 15
//comp bt = there are no missing elements in the array in between other elements, at the end OK = last level filled from left to right
//heap = comp bt
//min heap = all node have value lesser than their descedents
//min heap = all node have value greater than their descedents
typedef struct MaxHeap {
    int size;
    int* key_index;
    int* heap;
} MaxHeap;

const MaxHeap maxheap_init = { .size = 0, .key_index = NULL, .heap = NULL };
int INF = 1000, N_INF = -1000;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int i) { 
    return (i-1)/2; 
}
  
int left(int i) { 
    return (2*i + 1); 
}
  
int right(int i) { 
    return (2*i + 2);
}

int get_max(MaxHeap* max_heap) { //O(1)
    return max_heap->heap[0];
}

int is_empty(MaxHeap* max_heap) {
    return max_heap->heap[0] == N_INF;
}

int height(MaxHeap* max_heap) {
    return floor(log2(max_heap->size));
}

MaxHeap* create_maxheap(int size) {
    MaxHeap* heap = calloc(size, sizeof * heap);
    if (!heap) return heap;
    heap->size = size;
    return heap;
}

// floor(n/2) + 1 to n are leaves, thus 1 to floor(n/2) are non-leaves, for 0_index, it's 0 to floor(n/2) - 1!
void max_heapify(int* data, int* key_index, int i, int size) { // O(n), right to left, downward adjustment, instead of create_heap, which is O(nlog(n))
    int largest = i, leftie = left(i), rightie = right(i);
    
    if (leftie < size && data[leftie] > data[largest])
      largest = leftie;
    if (rightie < size && data[rightie] > data[largest])
      largest = rightie;//transitvity 
    if (largest != i) {
      swap(&data[i], &data[largest]);
      //
      key_index[data[i]] = i;
      key_index[data[largest]] = largest;
      //
      max_heapify(data, key_index, largest, size);
    }
}

void increase_key(MaxHeap* max_heap, int j, int key) {
    if (key < max_heap->heap[j])
        printf("Error: key must be larger \n");
    else {
        max_heap->heap[j] = key;
        max_heap->key_index[key] = j;
        for (int i = j; i > 0; i = parent(i)) {
            if (max_heap->heap[i] > max_heap->heap[parent(i)]) {
                swap(&max_heap->heap[i], &max_heap->heap[parent(i)]);
                max_heap->key_index[max_heap->heap[i]] = i;
                max_heap->key_index[max_heap->heap[parent(i)]] = parent(i);
            }
            else break;
        }
    }
}

int* build_max_heap(int* data, int* key_index, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        max_heapify(data, key_index, i, size);
    }
    return data;
}

void insert(MaxHeap* max_heap, int key) { // O(logn)
    int* temp = realloc (max_heap->heap, (max_heap->size + 1) * sizeof (*(max_heap->heap)));
    if (temp) {
        max_heap->heap = temp;
        max_heap->heap[max_heap->size] = N_INF;
        increase_key(max_heap, max_heap->size, key);
        max_heap->size += 1;
    }
}

void extract_max(MaxHeap* max_heap) { // O(logn)
    swap(&max_heap->heap[0], &max_heap->heap[max_heap->size - 1]);
    max_heap->size -= 1;
    max_heapify(max_heap->heap, max_heap->key_index, 0, max_heap->size);
}

void delete_key(MaxHeap* max_heap, int key) { //O(logn)
    int index = max_heap->key_index[key];
    increase_key(max_heap, index, INF);
    extract_max(max_heap);
}

void clear(MaxHeap* max_heap) {
    for (int i = 0; i < max_heap->size; i++)
        max_heap->heap[i] = N_INF; //shall never be in the heap
    max_heap->size = 0;
}

void print_heap(MaxHeap* max_heap) { // O(n)
    int size = max_heap->size;
    if (size % 2 != 0) 
        size = size + 1;
    for (int i = 0; i < size/2 - 1; i++) {
        printf("Parent: %d -> Left: %d | Right: %d \n", max_heap->heap[i], max_heap->heap[left(i)], max_heap->heap[right(i)]);
    }
    int j = max_heap->size/2 - 1;
    if (max_heap->size % 2 == 0)
        printf("Parent: %d -> Left: %d \n", max_heap->heap[j], max_heap->heap[left(j)]);
}

int is_maxheap(MaxHeap* max_heap) { // O(n)
    for (int i = max_heap->size-1; i > 0 ; i--) {
        if (max_heap->heap[i] > max_heap->heap[parent(i)]) {
            return 0;
        }
    }
    return 1;
}

void heap_sort(MaxHeap* max_heap) { // assumes already a max heap as argument
    // if the input is not a heap, call build_max_heap()
    for (int i = max_heap->size - 1; i >= 0; i--) {
        swap(&max_heap->heap[0], &max_heap->heap[i]);
        max_heapify(max_heap->heap, max_heap->key_index, 0, i); // 0 because we need only one downshift from root as we already have a heap: transitivty
    }
}

void print_arr(MaxHeap* max_heap) {
    for (int i = 0; i < max_heap->size; i++)
        printf("%d ", max_heap->heap[i]);
    printf("\n");
}

int main() {
    
    int size = 6, MAX_Key = INF, elements[6] = {10, 20, 15, 12, 40, 25};
    int* data = calloc(size, sizeof * data);
    if (!data) return 0;
    for (int i = 0; i < size; i++)
        data[i] = elements[i];
    int* key_index = calloc(MAX_Key, sizeof * key_index); // we assume keys > 0, if key < 0, then we add an additional array for negative values
    // int* key_index_n = calloc(MAX_Key, sizeof * key_index_n);
    if (!key_index) return 0;
    for (int i = 0; i < size; i++)
        key_index[elements[i]] = i;
        
    MaxHeap max_heap = maxheap_init;
    max_heap.size = size;
    max_heap.key_index = key_index;
    max_heap.heap = build_max_heap(data, key_index, size);
    
    print_heap(&max_heap);
    printf("is_max_heap: %d \n", is_maxheap(&max_heap));
    extract_max(&max_heap);
    printf("is_max_heap: %d \n", is_maxheap(&max_heap));
    print_heap(&max_heap);
    insert(&max_heap, 14);
    printf("is_max_heap: %d \n", is_maxheap(&max_heap));
    print_heap(&max_heap);
    delete_key(&max_heap, 14);
    printf("is_max_heap: %d \n", is_maxheap(&max_heap));
    print_heap(&max_heap);
    printf("height: %d \n", height(&max_heap));
    printf("max: %d \n", get_max(&max_heap));
    heap_sort(&max_heap);
    print_arr(&max_heap);
    clear(&max_heap);
    printf("is empty: %d \n", is_empty(&max_heap));
    
    free(data);
    free(key_index);
    
    return 0;
}








