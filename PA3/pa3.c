#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPER_NUM 10000 /**< The maximum number of operations */
#define MAX_NODE_NUM 500   /**< The maximum number of nodes */
#define INT_MAX 2147483647 /**< Represent infinity in key value */

typedef struct vertex {
  int name;             // name of the vertex
  int key;              // key value used in priority queue
  struct vertex *next;  // pointer to the next vertex in the linked list
} vertex;

typedef struct priorityQueue {
  int size;                        // size of the priority queue
  vertex *arr[MAX_NODE_NUM];       // array of vertices in the priority queue
  int position[MAX_NODE_NUM + 1];  // index is the name of vertex, value is the
                                   // index of vertex in arr
} priorityQueue;

/**
 * Initializes the priority queue with the given number of nodes.
 */
void initQueue(priorityQueue *pq, int nodeNum) {
  pq->size = nodeNum;
  for (int i = 0; i < nodeNum; i++) {
    pq->arr[i] = (vertex *)malloc(sizeof(vertex));
    pq->arr[i]->name = i + 1;   // set name of vertex
    pq->arr[i]->key = INT_MAX;  // set key to infinity
    pq->position[i + 1] = i;    // reset position of vertex
  }
}

/**
 * Initializes the vertex linked list by setting all elements to NULL.
 */
void initVertexLinkedList(vertex *vertexLinkedList[], int nodeNum) {
  for (int i = 1; i <= nodeNum; i++) {
    vertexLinkedList[i] = NULL;
  }
}

/**
 * Swaps the values of two vertex pointers.
 */
void swap(vertex **a, vertex **b) {
  vertex *temp = *a;
  *a = *b;
  *b = temp;
}

/**
 * Performs the heapify up operation on the priority queue.
 */
void heapifyUp(priorityQueue *pq, int index) {
  if (index == 0) {
    return;
  }
  int parent = (index - 1) / 2;

  // if parent is greater than child, swap them
  if (pq->arr[parent]->key > pq->arr[index]->key) {
    swap(&pq->arr[parent], &pq->arr[index]);

    // update position of vertices
    pq->position[pq->arr[parent]->name] = parent;
    pq->position[pq->arr[index]->name] = index;
    heapifyUp(pq, parent);
  }
}

/**
 * Performs the heapify down operation on the priority queue.
 */
void heapifyDown(priorityQueue *pq, int index) {
  int left = index * 2 + 1;
  int right = index * 2 + 2;
  int smallest = index;

  // find the smallest child
  if (left < pq->size && pq->arr[left]->key < pq->arr[smallest]->key) {
    smallest = left;
  }
  if (right < pq->size && pq->arr[right]->key < pq->arr[smallest]->key) {
    smallest = right;
  }

  // if smallest child is not the parent, swap them
  if (smallest != index) {
    swap(&pq->arr[smallest], &pq->arr[index]);

    // update position of vertices
    pq->position[pq->arr[smallest]->name] = smallest;
    pq->position[pq->arr[index]->name] = index;
    heapifyDown(pq, smallest);
  }
}

/**
 * Extracts the minimum element from the priority queue.
 */
vertex *extractMin(priorityQueue *pq) {
  // if priority queue is empty, return NULL
  if (pq->size == 0) {
    return NULL;
  }
  vertex *min = pq->arr[0];
  pq->arr[0] = pq->arr[pq->size - 1];

  // update position of vertices
  pq->position[pq->arr[0]->name] = 0;

  pq->size--;
  heapifyDown(pq, 0);
  return min;
}

/**
 * Inserts an edge into the graph.
 */
void insertEdge(int *weight[], int *numbers, vertex *vertexLinkedList[]) {
  // if edge does not exist, insert it
  if (weight[numbers[0]][numbers[1]] == 0) {
    // set weight of edge in weight matrix
    weight[numbers[0]][numbers[1]] = numbers[2];
    weight[numbers[1]][numbers[0]] = numbers[2];

    // insert vertex into linked list head
    vertex *newVertex = (vertex *)malloc(sizeof(vertex));
    newVertex->name = numbers[1];
    newVertex->key = numbers[2];

    if (vertexLinkedList[numbers[0]] != NULL) {
      vertex *temp = vertexLinkedList[numbers[0]];
      vertexLinkedList[numbers[0]] = newVertex;
      newVertex->next = temp;
    } else {
      newVertex->next = NULL;
      vertexLinkedList[numbers[0]] = newVertex;
    }

    // insert the opposite direction
    newVertex = (vertex *)malloc(sizeof(vertex));
    newVertex->name = numbers[0];
    newVertex->key = numbers[2];

    if (vertexLinkedList[numbers[1]] != NULL) {
      vertex *temp = vertexLinkedList[numbers[1]];
      vertexLinkedList[numbers[1]] = newVertex;
      newVertex->next = temp;
    } else {
      newVertex->next = NULL;
      vertexLinkedList[numbers[1]] = newVertex;
    }
  }
}

/**
 * Deletes an edge from the graph.
 */
void deleteEdge(int *weight[], int *numbers, vertex *vertexLinkedList[]) {
  // if edge exists, delete it
  if (weight[numbers[0]][numbers[1]] != 0) {
    // set weight of edge in weight matrix
    weight[numbers[0]][numbers[1]] = 0;
    weight[numbers[1]][numbers[0]] = 0;

    // delete vertex from linked list
    vertex *temp = vertexLinkedList[numbers[0]];
    vertex *prev = NULL;
    while (temp != NULL) {
      if (temp->name == numbers[1]) {
        if (prev == NULL) {
          vertexLinkedList[numbers[0]] = temp->next;
        } else {
          prev->next = temp->next;
        }
        free(temp);
        break;
      }
      prev = temp;
      temp = temp->next;
    }

    // delete the opposite direction
    temp = vertexLinkedList[numbers[1]];
    prev = NULL;
    while (temp != NULL) {
      if (temp->name == numbers[0]) {
        if (prev == NULL) {
          vertexLinkedList[numbers[1]] = temp->next;
        } else {
          prev->next = temp->next;
        }
        free(temp);
        break;
      }
      prev = temp;
      temp = temp->next;
    }
  }
}

/**
 * Changes the weight of an edge in the graph.
 */
void changeWeight(int *weight[], int *numbers, vertex *vertexLinkedList[]) {
  // if edge exists, change its weight
  if (weight[numbers[0]][numbers[1]] != 0) {
    // set weight of edge in weight matrix
    weight[numbers[0]][numbers[1]] = numbers[2];
    weight[numbers[1]][numbers[0]] = numbers[2];

    // change vertex in linked list
    vertex *temp = vertexLinkedList[numbers[0]];
    while (temp != NULL) {
      if (temp->name == numbers[1]) {
        temp->key = numbers[2];
        break;
      }
      temp = temp->next;
    }

    // change the opposite direction
    temp = vertexLinkedList[numbers[1]];
    while (temp != NULL) {
      if (temp->name == numbers[0]) {
        temp->key = numbers[2];
        break;
      }
      temp = temp->next;
    }
  }
}

/**
 * Finds the minimum spanning tree of the graph using Prim's algorithm.
 */
void findMST(priorityQueue *pq, int *weight[], int nodeNum,
             vertex *vertexLinkedList[], FILE *out) {
  int MST = 0;
  pq->arr[0]->key = 0;

  // array to check if the vertex is connected
  int connected[nodeNum];
  for (int i = 0; i < nodeNum; i++) {
    connected[i] = 0;
  }

  // check if the graph is disconnected
  while (pq->size != 0) {
    vertex *u = extractMin(pq);
    connected[u->name - 1] = 1;
    if (u->key == INT_MAX) {
      fprintf(out, "Disconnected\n");
      return;
    }
    MST += u->key;

    // update key of adjacent vertices
    vertex *temp = vertexLinkedList[u->name];
    while (temp != NULL) {
      int pos = pq->position[temp->name];
      if (connected[temp->name - 1] == 0 &&
          pq->arr[pos]->key > weight[u->name][temp->name]) {
        pq->arr[pos]->key = weight[u->name][temp->name];
        heapifyUp(pq, pos);
      }
      temp = temp->next;
    }
  }
  // write MST to output file
  fprintf(out, "%d\n", MST);
}

int main() {

  // open input and output files
  FILE *file = fopen("mst.in", "r");
  FILE *out = fopen("mst.out", "w");

  // read the number of nodes
  int nodeNum = 0;
  fscanf(file, "%d", &nodeNum);

  // initialize weight matrix
  int **weight = (int **)malloc(sizeof(int *) * (nodeNum + 1));
  for (int i = 0; i <= nodeNum; i++) {
    weight[i] = (int *)malloc(sizeof(int) * (nodeNum + 1));
    for (int j = 0; j <= nodeNum; j++) {
      weight[i][j] = 0;
    }
  }

  // initialize priority queue
  priorityQueue pq;
  initQueue(&pq, nodeNum);

  //  initialize vertex linked list
  vertex *vertexLinkedList[MAX_NODE_NUM + 1];
  initVertexLinkedList(vertexLinkedList, nodeNum);

  char oper[15];   // for storing operation temporarily
  int numbers[3];  // for storing numbers temporarily

  // read operations from input file and perform them
  while (fscanf(file, "%s", oper) != EOF) {
    if (strcmp(oper, "insertEdge") == 0) {
      fscanf(file, "%d %d %d", &numbers[0], &numbers[1], &numbers[2]);
      insertEdge(weight, numbers, vertexLinkedList);
    } else if (strcmp(oper, "changeWeight") == 0) {
      fscanf(file, "%d %d %d", &numbers[0], &numbers[1], &numbers[2]);
      changeWeight(weight, numbers, vertexLinkedList);
    } else if (strcmp(oper, "deleteEdge") == 0) {
      fscanf(file, "%d %d", &numbers[0], &numbers[1]);
      deleteEdge(weight, numbers, vertexLinkedList);
    } else if (strcmp(oper, "findMST") == 0) {
      initQueue(&pq,
                nodeNum);  // reset priority queue for each findMST operation
      findMST(&pq, weight, nodeNum, vertexLinkedList, out);
    }
  }

  // close input and output files
  fclose(file);
  fclose(out);

  return 0;
}
