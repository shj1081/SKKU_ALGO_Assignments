#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_OPER_NUM 10000
#define MAX_NODE_NUM 500
#define INT_MAX 2147483647

typedef struct vertex {
  int name;
  int key;
  struct vertex *parent;
} vertex;

typedef struct priorityQueue {
  int size;
  vertex *arr[MAX_NODE_NUM];
  int position[MAX_NODE_NUM + 1];  // index is the name of vertex, value is the
                                   // index of vertex in arr
} priorityQueue;

void initQueue(priorityQueue *pq, int nodeNum) {
  pq->size = nodeNum;
  for (int i = 0; i < nodeNum; i++) {
    pq->arr[i] = (vertex *)malloc(sizeof(vertex));
    pq->arr[i]->name = i + 1;
    pq->arr[i]->key = INT_MAX;
    pq->position[i + 1] = i;
  }
}

void swap(vertex **a, vertex **b) {
  vertex *temp = *a;
  *a = *b;
  *b = temp;
}

void heapifyUp(priorityQueue *pq, int index) {
  if (index == 0) {
    return;
  }
  int parent = (index - 1) / 2;
  if (pq->arr[parent]->key > pq->arr[index]->key) {
    swap(&pq->arr[parent], &pq->arr[index]);
    pq->position[pq->arr[parent]->name] = parent;
    pq->position[pq->arr[index]->name] = index;
    heapifyUp(pq, parent);
  }
}

void heapifyDown(priorityQueue *pq, int index) {
  int left = index * 2 + 1;
  int right = index * 2 + 2;
  int smallest = index;
  if (left < pq->size && pq->arr[left]->key < pq->arr[smallest]->key) {
    smallest = left;
  }
  if (right < pq->size && pq->arr[right]->key < pq->arr[smallest]->key) {
    smallest = right;
  }
  if (smallest != index) {
    swap(&pq->arr[smallest], &pq->arr[index]);
    pq->position[pq->arr[smallest]->name] = smallest;
    pq->position[pq->arr[index]->name] = index;
    heapifyDown(pq, smallest);
  }
}

vertex *extractMin(priorityQueue *pq) {
  if (pq->size == 0) {
    return NULL;
  }
  vertex *min = pq->arr[0];
  pq->arr[0] = pq->arr[pq->size - 1];
  pq->size--;
  heapifyDown(pq, 0);
  return min;
}

void insertEdge(int *weight[], int *numbers) {
  if (weight[numbers[0]][numbers[1]] == 0) {
    weight[numbers[0]][numbers[1]] = numbers[2];
    weight[numbers[1]][numbers[0]] = numbers[2];
  }
}

void deleteEdge(int *weight[], int *numbers) {
  if (weight[numbers[0]][numbers[1]] != 0) {
    weight[numbers[0]][numbers[1]] = 0;
    weight[numbers[1]][numbers[0]] = 0;
  }
}

void changeWeight(int *weight[], int *numbers) {
  if (weight[numbers[0]][numbers[1]] != 0) {
    weight[numbers[0]][numbers[1]] = numbers[2];
    weight[numbers[1]][numbers[0]] = numbers[2];
  }
}

int *adj(int *weight[], int name, int nodeNum, int *adjNum, int *connectd) {
  int *adjArr = (int *)malloc(sizeof(int) * nodeNum);
  for (int i = 1; i <= nodeNum; i++) {
    if (connectd[i-1] == 0 && weight[name][i] != 0) {
      adjArr[*adjNum] = i;
      (*adjNum)++;
    }
  }
  return adjArr;
}

void findMST(priorityQueue *pq, int *weight[], int nodeNum) {
  int MST = 0;
  pq->arr[0]->key = 0;
  pq->arr[0]->parent = NULL;
  int connected[nodeNum];
  for (int i = 0; i < nodeNum; i++) {
    connected[i] = 0;
  }

  while (pq->size != 0) {
    vertex *u = extractMin(pq);
    connected[u->name-1] = 1;
    if (u->key == INT_MAX) {
      printf("Disconnected\n");
      return;
    }
    MST += u->key;
    int *adjArr;
    int adjNum = 0;
    adjArr = adj(weight, u->name, nodeNum, &adjNum, connected);

    for (int i = 0; i < adjNum; i++) {
      int v = adjArr[i];
      if (pq->arr[pq->position[v]]->key > weight[u->name][v]) {
        pq->arr[pq->position[v]]->key = weight[u->name][v];
        heapifyUp(pq, pq->position[v]);
      }
    }
  }
   printf("%d\n", MST);
}

int main() {

  // clock start
  clock_t start, end;
  double cpu_time_used;
  start = clock();


  FILE *file = fopen("mst_in.txt", "r");

  int operNum = 0;
  int nodeNum = 0;
  fscanf(file, "%d", &nodeNum);

  int **weight = (int **)malloc(sizeof(int *) * (nodeNum + 1));
  for (int i = 0; i <= nodeNum; i++) {
    weight[i] = (int *)malloc(sizeof(int) * (nodeNum + 1));
    for (int j = 0; j <= nodeNum; j++) {
      weight[i][j] = 0;
    }
  }

  priorityQueue pq;
  initQueue(&pq, nodeNum);

  char oper[15];  // for storing operation temporarily
  int numbers[3];

  while (operNum < MAX_OPER_NUM && fscanf(file, "%s", oper) != EOF) {
    if (strcmp(oper, "insertEdge") == 0) {
      fscanf(file, "%d %d %d", &numbers[0], &numbers[1], &numbers[2]);
      insertEdge(weight, numbers);
    } else if (strcmp(oper, "changeWeight") == 0) {
      fscanf(file, "%d %d %d", &numbers[0], &numbers[1], &numbers[2]);
      changeWeight(weight, numbers);
    } else if (strcmp(oper, "deleteEdge") == 0) {
      fscanf(file, "%d %d", &numbers[0], &numbers[1]);
      deleteEdge(weight, numbers);
    } else {
      initQueue(&pq, nodeNum);
      findMST(&pq, weight, nodeNum);
    }
    operNum++;
  }

  fclose(file);

  // clock end
  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("time: %f\n", cpu_time_used);

  return 0;
}
