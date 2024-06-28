#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 501
#define MAX_EDGE 10001

typedef struct _edge {
  int v1, v2, w;
} EG;

typedef struct _interval_edge {
  int l, r;
  int v1, v2, w;
} IEG;

EG next[MAX_EDGE], nc_edge[MAX_EDGE], red_edge[MAX_EDGE];
IEG c_edge[MAX_EDGE];
int loc[MAX][MAX], nc_loc[MAX][MAX], c_loc[MAX][MAX];
// {nidx : next, ngidx : nc_edge, cgidx : c_edge, rdidx : red_edge, idx :
// query_f}
int nidx, ngidx, cgidx, rdidx;
int query_f[MAX], idx;

typedef struct _parent {
  int parray[MAX];
} PA;

void quicksort1(EG arr[], int left, int right) {
  int L = left, R = right;
  int pivot = arr[(left + right) / 2].w;
  EG tmp;
  while (L <= R) {
    while (arr[L].w < pivot) L++;
    while (arr[R].w > pivot) R--;
    if (L <= R) {
      if (L < R) {
        tmp.v1 = arr[L].v1, tmp.v2 = arr[L].v2, tmp.w = arr[L].w;
        arr[L].v1 = arr[R].v1, arr[L].v2 = arr[R].v2, arr[L].w = arr[R].w;
        arr[R].v1 = tmp.v1, arr[R].v2 = tmp.v2, arr[R].w = tmp.w;
      }
      L++, R--;
    }
  }
  if (L < right) quicksort1(arr, L, right);
  if (left < R) quicksort1(arr, left, R);
}

void quicksort2(IEG arr[], int left, int right) {
  int L = left, R = right;
  int pivot = arr[(left + right) / 2].w;
  IEG tmp;
  while (L <= R) {
    while (arr[L].w < pivot) L++;
    while (arr[R].w > pivot) R--;
    if (L <= R) {
      if (L < R) {
        tmp.v1 = arr[L].v1, tmp.v2 = arr[L].v2, tmp.w = arr[L].w,
        tmp.l = arr[L].l, tmp.r = arr[L].r;
        arr[L].v1 = arr[R].v1, arr[L].v2 = arr[R].v2, arr[L].w = arr[R].w,
        arr[L].l = arr[R].l, arr[L].r = arr[R].r;
        arr[R].v1 = tmp.v1, arr[R].v2 = tmp.v2, arr[R].w = tmp.w,
        arr[R].l = tmp.l, arr[R].r = tmp.r;
      }
      L++, R--;
    }
  }
  if (L < right) quicksort2(arr, L, right);
  if (left < R) quicksort2(arr, left, R);
}

int find_parent(int parent[], int v) {
  if (parent[v] <= 0) return v;
  return parent[v] = find_parent(parent, parent[v]);
}

void union_set(int parent[], int u, int v) { parent[v] = u; }

void reduction(PA x) {
  if (ngidx <= 0) return;
  quicksort1(nc_edge, 0, ngidx - 1);
  int pa, pb;
  rdidx = 0;
  for (int i = 0; i < ngidx; i++) {
    pa = find_parent(x.parray, nc_edge[i].v1);
    pb = find_parent(x.parray, nc_edge[i].v2);
    if (pa != pb) {
      union_set(x.parray, pa, pb);
      red_edge[rdidx++] = nc_edge[i];
    }
  }
}

int cnt, fixed_val;

void contraction(int arr[], int n) {
  int* parent = (int*)calloc(n + 1, sizeof(int));
  cnt = 0, fixed_val = 0;
  int index = cgidx;
  int a, b, pa, pb;
  for (int i = 0; i < cgidx; i++) {
    a = c_edge[i].v1;
    b = c_edge[i].v2;
    pa = find_parent(parent, a);
    pb = find_parent(parent, b);
    if (pa != pb) union_set(parent, pa, pb);
  }
  for (int i = 0; i < rdidx; i++) {
    a = red_edge[i].v1;
    b = red_edge[i].v2;
    pa = find_parent(parent, a);
    pb = find_parent(parent, b);
    if (pa != pb) {
      union_set(parent, pa, pb);
      pa = find_parent(arr, a);
      pb = find_parent(arr, b);
      if (pa != pb) union_set(arr, pa, pb);
      fixed_val += red_edge[i].w;
      cnt++;
    } else {
      c_edge[cgidx].v1 = a, c_edge[cgidx].v2 = b,
      c_edge[cgidx].w = red_edge[i].w, c_edge[cgidx].l = 1, c_edge[cgidx].r = n;
      cgidx++;
    }
  }
  free(parent);
}

void find_MST(FILE* fp, PA x, int k, int goal) {
  int count = 0, ans = 0;
  int a, b, pa, pb;
  for (int i = 0; i < cgidx; i++) {
    if (c_edge[i].l <= k && k <= c_edge[i].r) {
      a = c_edge[i].v1;
      b = c_edge[i].v2;
      pa = find_parent(x.parray, a);
      pb = find_parent(x.parray, b);
      if (pa != pb) {
        union_set(x.parray, pa, pb);
        ans += c_edge[i].w;
        count++;
      }
    }
    if (count == goal) {
      fprintf(fp, "%d\n", ans + fixed_val);
      return;
    }
  }
  fprintf(fp, "Disconnected\n");
  return;
}

int main() {
  FILE* fin = fopen("mst.in", "r");
  FILE* fout = fopen("mst.out", "w");
  int TIME = 0;
  /* Timer on */
  clock_t start = clock();
  int N;
  char operation[20];
  fscanf(fin, "%d", &N);
  memset(loc, -1, sizeof(loc));
  fgets(operation, sizeof(operation), fin);
  int v1, v2, w, tmp, nv1, nv2, nw, xw;
  int i;
  PA parent;
  while (!feof(fin)) {
    // nc_loc, c_loc 초기화, nc_edge, c_edge, query_f 초기화 -> N번 마다 반복
    memset(nc_loc, -1, sizeof(nc_loc));
    memset(c_loc, -1, sizeof(c_loc));
    ngidx = 0, cgidx = 0, idx = 0;
    // next -> nc_edge로 copy
    for (i = 0; i < nidx; i++) {
      nc_edge[i].v1 = next[i].v1, nc_edge[i].v2 = next[i].v2,
      nc_edge[i].w = next[i].w;
      v1 = nc_edge[i].v1, v2 = nc_edge[i].v2;
      nc_loc[v1][v2] = i, nc_loc[v2][v1] = i;
    }
    ngidx = nidx;
    for (i = 1; i <= N && !feof(fin); i++) {
      fscanf(fin, "%s", operation);
      switch (operation[0]) {
        case 'i':
          fscanf(fin, "%d %d %d", &v1, &v2, &w);
          if (loc[v1][v2] == -1) {
            // next 업데이트
            next[nidx].v1 = v1, next[nidx].v2 = v2, next[nidx].w = w;
            loc[v1][v2] = nidx, loc[v2][v1] = nidx;
            nidx++;
            // nc_edge 업데이트
            tmp = nc_loc[v1][v2];
            if (tmp != -1) {
              nc_loc[v1][v2] = -1, nc_loc[v2][v1] = -1;
              ngidx--;
              nv1 = nc_edge[ngidx].v1, nv2 = nc_edge[ngidx].v2,
              nw = nc_edge[ngidx].w;
              nc_edge[tmp].v1 = nv1, nc_edge[tmp].v2 = nv2, nc_edge[tmp].w = nw;
              nc_loc[nv1][nv2] = tmp, nc_loc[nv2][nv1] = tmp;
            }
            // c_edge 업데이트
            c_edge[cgidx].v1 = v1, c_edge[cgidx].v2 = v2, c_edge[cgidx].w = w,
            c_edge[cgidx].l = i, c_edge[cgidx].r = N;
            c_loc[v1][v2] = cgidx, c_loc[v2][v1] = cgidx;
            cgidx++;
          }
          break;
        case 'd':
          fscanf(fin, "%d %d", &v1, &v2);
          tmp = loc[v1][v2];
          if (tmp != -1) {
            w = next[tmp].w;
            // next 업데이트
            loc[v1][v2] = -1, loc[v2][v1] = -1;
            nidx--;
            nv1 = next[nidx].v1, nv2 = next[nidx].v2, nw = next[nidx].w;
            next[tmp].v1 = nv1, next[tmp].v2 = nv2, next[tmp].w = nw;
            loc[nv1][nv2] = tmp, loc[nv2][nv1] = tmp;
            // nc_edge 업데이트
            tmp = nc_loc[v1][v2];
            if (tmp != -1) {
              nc_loc[v1][v2] = -1, nc_loc[v2][v1] = -1;
              ngidx--;
              nv1 = nc_edge[ngidx].v1, nv2 = nc_edge[ngidx].v2,
              nw = nc_edge[ngidx].w;
              nc_edge[tmp].v1 = nv1, nc_edge[tmp].v2 = nv2, nc_edge[tmp].w = nw;
              nc_loc[nv1][nv2] = tmp, nc_loc[nv2][nv1] = tmp;
            }
            // c_edge 업데이트
            tmp = c_loc[v1][v2];
            if (tmp == -1) {
              c_edge[cgidx].v1 = v1, c_edge[cgidx].v2 = v2, c_edge[cgidx].w = w,
              c_edge[cgidx].l = 1, c_edge[cgidx].r = i - 1;
              c_loc[v1][v2] = cgidx, c_loc[v2][v1] = cgidx;
              cgidx++;
            }
            // c_edge가 있는 경우 -> right time을 변경
            else
              c_edge[tmp].r = i - 1;
          }
          break;
        case 'c':
          fscanf(fin, "%d %d %d", &v1, &v2, &w);
          tmp = loc[v1][v2];
          if (tmp != -1) {
            // next 업데이트
            xw = next[tmp].w;
            next[tmp].w = w;
            // nc_edge 업데이트
            tmp = nc_loc[v1][v2];
            if (tmp != -1) {
              nc_loc[v1][v2] = -1, nc_loc[v2][v1] = -1;
              ngidx--;
              nv1 = nc_edge[ngidx].v1, nv2 = nc_edge[ngidx].v2,
              nw = nc_edge[ngidx].w;
              nc_edge[tmp].v1 = nv1, nc_edge[tmp].v2 = nv2, nc_edge[tmp].w = nw;
              nc_loc[nv1][nv2] = tmp, nc_loc[nv2][nv1] = tmp;
            }
            // c_edge 업데이트
            tmp = c_loc[v1][v2];
            if (tmp == -1) {
              c_edge[cgidx].v1 = v1, c_edge[cgidx].v2 = v2,
              c_edge[cgidx].w = xw, c_edge[cgidx].l = 1,
              c_edge[cgidx].r = i - 1;
              cgidx++;
              c_edge[cgidx].v1 = v1, c_edge[cgidx].v2 = v2, c_edge[cgidx].w = w,
              c_edge[cgidx].l = i, c_edge[cgidx].r = N;
              c_loc[v1][v2] = cgidx, c_loc[v2][v1] = cgidx;
              cgidx++;
            } else {
              c_edge[tmp].r = i - 1;
              c_edge[cgidx].v1 = v1, c_edge[cgidx].v2 = v2, c_edge[cgidx].w = w,
              c_edge[cgidx].l = i, c_edge[cgidx].r = N;
              c_loc[v1][v2] = cgidx, c_loc[v2][v1] = cgidx;
              cgidx++;
            }
          }
          break;
        case 'f':
          query_f[idx++] = i;
          break;
      }
    }
    // MST
    memset(parent.parray, -1, sizeof(parent.parray));
    reduction(parent);
    contraction(parent.parray, N);
    quicksort2(c_edge, 0, cgidx - 1);
    if (cnt == N - 1) {
      for (int i = 0; i < idx; i++) fprintf(fout, "%d", fixed_val);
    } else {
      for (i = 0; i < idx; i++) find_MST(fout, parent, query_f[i], N - 1 - cnt);
    }
  }

  TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
  printf("\nTIME : %d ms\n", TIME);
  return 0;
}