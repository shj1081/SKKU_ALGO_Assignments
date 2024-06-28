#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCES 5
#define MAX_LEN 120

// Return the sum of the given integer array
int sum(int *arr, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  return sum;
}

int max(int a, int b) { return (a > b) ? a : b; }

// Return the maximum value of the given integer array
int max_multi(int *arr, int size) {
  int max = arr[0];
  for (int i = 1; i < size; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

// Read the input file and store the sequences in seqs
void read_input_file(char **seqs, int *seqs_num) {
  FILE *file = fopen("input.txt", "r");
  // get the first line as seqs_num
  fscanf(file, "%d", seqs_num);
  // ignore the second line with dollar sign
  fscanf(file, "%*s");
  // read the sequences from 3rd line, and store them in seqs
  for (int i = 0; i < *seqs_num; i++) {
    seqs[i] = malloc(sizeof(char) * MAX_LEN);
    fscanf(file, "%s", seqs[i]);
  }
}

void MSA_write_output_file(char **seqs, int seqs_num, char *lcs) {
  int match[seqs_num];
  char aligned_seqs[seqs_num][MAX_LEN * seqs_num + 1];
  int seqs_idx[seqs_num];
  memset(seqs_idx, 0, sizeof(seqs_idx));
  int aligned_idx[seqs_num];
  memset(aligned_idx, 0, sizeof(aligned_idx));
  int lcs_len = strlen(lcs);
  int lcs_idx = 0;

  // Initialize the aligned sequences with '-'
  for (int i = 0; i < seqs_num; i++) {
    memset(aligned_seqs[i], '-', sizeof(aligned_seqs[i]));
  }

  // align the sequences
  while (lcs_idx < lcs_len) {
    // Check match or not
    for (int i = 0; i < seqs_num; i++) {
      if (seqs[i][seqs_idx[i]] == lcs[lcs_idx]) {
        match[i] = 1;
      } else {
        match[i] = 0;
      }
    }

    // If all match or all mismatch
    if (sum(match, seqs_num) == seqs_num || sum(match, seqs_num) == 0) {
      for (int i = 0; i < seqs_num; i++) {
        if (seqs_idx[i] < strlen(seqs[i])) {
          aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
          seqs_idx[i]++;
        }
        aligned_idx[i]++;
      }
      if (sum(match, seqs_num) == seqs_num) {
        lcs_idx++;
      }
    }

    // If partial match
    else {
      for (int i = 0; i < seqs_num; i++) {
        if (match[i] != 1) {
          aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
          seqs_idx[i]++;
        }
        // Increment alignment index for all cases
        aligned_idx[i]++;
      }
    }
  }

  // Fill in the remaining characters
  for (int i = 0; i < seqs_num; i++) {
    while (seqs_idx[i] < strlen(seqs[i])) {
      aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
      seqs_idx[i]++;
      aligned_idx[i]++;
    }
  }

  // adjust the length of aligned sequences
  int max_alginment_length = max_multi(aligned_idx, seqs_num);
  printf("%d\n", max_alginment_length);
  for (int i = 0; i < seqs_num; i++) {
    printf("%d\n", aligned_idx[i]);
  }

  for (int i = 0; i < seqs_num; i++) {
    aligned_idx[i] = max_alginment_length;
    aligned_seqs[i][max_alginment_length] = '\0';
  }

  // write the output file of the aligned sequences
  FILE *file = fopen("output2.txt", "w");
  for (int i = 0; i < seqs_num; i++) {
    fprintf(file, "%s\n", aligned_seqs[i]);
  }

  // Write "*" for the matched positions
  for (int j = 0; j < max_alginment_length; j++) {
    char c = aligned_seqs[0][j];
    int match = 1;
    for (int i = 1; i < seqs_num; i++) {
      if (aligned_seqs[i][j] != c) {
        match = 0;
        break;
      }
    }
    fputc(match ? '*' : ' ', file);
  }
  fclose(file);
}

char *lcs_2(char **seqs) {
  int len1 = strlen(seqs[0]);
  int len2 = strlen(seqs[1]);

  int **matrix = (int **)malloc((len1 + 1) * sizeof(int *));
  for (int i = 0; i <= len1; i++) {
    matrix[i] = (int *)malloc((len2 + 1) * sizeof(int));
  }

  for (int idx1 = 0; idx1 <= len1; idx1++) {
    for (int idx2 = 0; idx2 <= len2; idx2++) {
      if (idx1 == 0 || idx2 == 0) {
        matrix[idx1][idx2] = 0;
      } else if (seqs[0][idx1 - 1] == seqs[1][idx2 - 1]) {
        matrix[idx1][idx2] = matrix[idx1 - 1][idx2 - 1] + 1;
      } else {
        matrix[idx1][idx2] =
            max(matrix[idx1 - 1][idx2], matrix[idx1][idx2 - 1]);
      }
    }
  }

  int i = len1;
  int j = len2;
  int length = matrix[i][j];
  char *lcs = (char *)malloc((length + 1) * sizeof(char));
  lcs[length] = '\0';

  while (i > 0 && j > 0) {
    if ((seqs[0][i - 1] == seqs[1][j - 1]) &&
        (matrix[i][j] == matrix[i - 1][j - 1] + 1)) {
      lcs[length - 1] = seqs[0][i - 1];  // Corrected sequence assignment
      i--;
      j--;
      length--;
    } else if (matrix[i - 1][j] >= matrix[i][j - 1]) {
      i--;
    } else {
      j--;
    }
  }

  // Free the dynamically allocated matrix memory
  for (int i = 0; i <= len1; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return lcs;
}

char *lcs_3(char **seqs) {
  int len1 = strlen(seqs[0]);
  int len2 = strlen(seqs[1]);
  int len3 = strlen(seqs[2]);

  int ***matrix = (int ***)malloc((len1 + 1) * sizeof(int **));
  for (int i = 0; i <= len1; i++) {
    matrix[i] = (int **)malloc((len2 + 1) * sizeof(int *));
    for (int j = 0; j <= len2; j++) {
      matrix[i][j] = (int *)malloc((len3 + 1) * sizeof(int));
    }
  }

  for (int idx1 = 0; idx1 <= len1; idx1++) {
    for (int idx2 = 0; idx2 <= len2; idx2++) {
      for (int idx3 = 0; idx3 <= len3; idx3++) {
        if (idx1 == 0 || idx2 == 0 || idx3 == 0) {
          matrix[idx1][idx2][idx3] = 0;
        } else if (seqs[0][idx1 - 1] == seqs[1][idx2 - 1] &&
                   seqs[0][idx1 - 1] == seqs[2][idx3 - 1]) {
          matrix[idx1][idx2][idx3] = matrix[idx1 - 1][idx2 - 1][idx3 - 1] + 1;
        } else {
          matrix[idx1][idx2][idx3] = max(
              max(matrix[idx1 - 1][idx2][idx3], matrix[idx1][idx2 - 1][idx3]),
              matrix[idx1][idx2][idx3 - 1]);
        }
      }
    }
  }

  int i = len1;
  int j = len2;
  int k = len3;
  int length = matrix[i][j][k];
  char *lcs = (char *)malloc((length + 1) * sizeof(char));
  lcs[length] = '\0';

  while (i > 0 && j > 0 && k > 0) {
    if ((matrix[i][j][k] == matrix[i - 1][j - 1][k - 1] + 1) &&
        (seqs[0][i - 1] == seqs[1][j - 1]) &&
        (seqs[0][i - 1] == seqs[2][k - 1])) {
      lcs[length - 1] = seqs[0][i - 1];
      i--;
      j--;
      k--;
      length--;
    } else if (matrix[i - 1][j][k] >= matrix[i][j - 1][k] &&
               matrix[i - 1][j][k] >= matrix[i][j][k - 1]) {
      i--;
    } else if (matrix[i][j - 1][k] >= matrix[i - 1][j][k] &&
               matrix[i][j - 1][k] >= matrix[i][j][k - 1]) {
      j--;
    } else {
      k--;
    }
  }

  // Free the dynamically allocated matrix memory
  for (int i = 0; i <= len1; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return lcs;
}

char *lcs_4(char **seqs) {
  int len1 = strlen(seqs[0]);
  int len2 = strlen(seqs[1]);
  int len3 = strlen(seqs[2]);
  int len4 = strlen(seqs[3]);

  int ****matrix = (int ****)malloc((len1 + 1) * sizeof(int ***));
  for (int i = 0; i <= len1; i++) {
    matrix[i] = (int ***)malloc((len2 + 1) * sizeof(int **));
    for (int j = 0; j <= len2; j++) {
      matrix[i][j] = (int **)malloc((len3 + 1) * sizeof(int *));
      for (int k = 0; k <= len3; k++) {
        matrix[i][j][k] = (int *)malloc((len4 + 1) * sizeof(int));
      }
    }
  }

  for (int idx1 = 0; idx1 <= len1; idx1++) {
    for (int idx2 = 0; idx2 <= len2; idx2++) {
      for (int idx3 = 0; idx3 <= len3; idx3++) {
        for (int idx4 = 0; idx4 <= len4; idx4++) {
          if (idx1 == 0 || idx2 == 0 || idx3 == 0 || idx4 == 0) {
            matrix[idx1][idx2][idx3][idx4] = 0;
          } else if (seqs[0][idx1 - 1] == seqs[1][idx2 - 1] &&
                     seqs[0][idx1 - 1] == seqs[2][idx3 - 1] &&
                     seqs[0][idx1 - 1] == seqs[3][idx4 - 1]) {
            matrix[idx1][idx2][idx3][idx4] =
                matrix[idx1 - 1][idx2 - 1][idx3 - 1][idx4 - 1] + 1;
          } else {
            matrix[idx1][idx2][idx3][idx4] =
                max(max(max(matrix[idx1 - 1][idx2][idx3][idx4],
                            matrix[idx1][idx2 - 1][idx3][idx4]),
                        matrix[idx1][idx2][idx3 - 1][idx4]),
                    matrix[idx1][idx2][idx3][idx4 - 1]);
          }
        }
      }
    }
  }

  int i = len1;
  int j = len2;
  int k = len3;
  int l = len4;
  int length = matrix[i][j][k][l];
  char *lcs = (char *)malloc((length + 1) * sizeof(char));
  lcs[length] = '\0';

  while (i > 0 && j > 0 && k > 0 && l > 0) {
    if ((matrix[i][j][k][l] == matrix[i - 1][j - 1][k - 1][l - 1] + 1) &&
        (seqs[0][i - 1] == seqs[1][j - 1]) &&
        (seqs[0][i - 1] == seqs[2][k - 1]) &&
        (seqs[0][i - 1] == seqs[3][l - 1])) {
      lcs[length - 1] = seqs[0][i - 1];
      i--;
      j--;
      k--;
      l--;
      length--;
    } else if (matrix[i - 1][j][k][l] >= matrix[i][j - 1][k][l] &&
               matrix[i - 1][j][k][l] >= matrix[i][j][k - 1][l] &&
               matrix[i - 1][j][k][l] >= matrix[i][j][k][l - 1]) {
      i--;
    } else if (matrix[i][j - 1][k][l] >= matrix[i - 1][j][k][l] &&
               matrix[i][j - 1][k][l] >= matrix[i][j][k - 1][l] &&
               matrix[i][j - 1][k][l] >= matrix[i][j][k][l - 1]) {
      j--;
    } else if (matrix[i][j][k - 1][l] >= matrix[i - 1][j][k][l] &&
               matrix[i][j][k - 1][l] >= matrix[i][j - 1][k][l] &&
               matrix[i][j][k - 1][l] >= matrix[i][j][k][l - 1]) {
      k--;
    } else {
      l--;
    }
  }

  // Free the dynamically allocated matrix memory
  for (int i = 0; i <= len1; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return lcs;
}

void recomputeDP_5(char **seqs, int len1, int len2, int len3, int len4, int len5, int *****L) {
  for (int i = 0; i <= len1; i++) {
    int idx = i % 2;
    for (int j = 0; j <= len2; j++) {
      for (int k = 0; k <= len3; k++) {
        for (int l = 0; l <= len4; l++) {
          for (int m = 0; m <= len5; m++) {
            if (i == 0 || j == 0 || k == 0 || l == 0 || m == 0)
              L[idx][j][k][l][m] = 0;
            else if (seqs[0][i - 1] == seqs[1][j - 1] &&
                     seqs[0][i - 1] == seqs[2][k - 1] &&
                     seqs[0][i - 1] == seqs[3][l - 1] &&
                     seqs[0][i - 1] == seqs[4][m - 1])
              L[idx][j][k][l][m] = L[1 - idx][j - 1][k - 1][l - 1][m - 1] + 1;
            else
              L[idx][j][k][l][m] =
                  max(L[1 - idx][j][k][l][m],
                      max(L[idx][j - 1][k][l][m],
                          max(L[idx][j][k - 1][l][m],
                              max(L[idx][j][k][l - 1][m], L[idx][j][k][l][m - 1]))));
          }
        }
      }
    }
  }
}

int lcsLEN_5(char **seqs, int len1, int len2, int len3, int len4, int len5, int *****L) {
  recomputeDP_5(seqs, len1, len2, len3, len4, len5, L);
  int idx = len1 % 2;
  return L[idx][len2][len3][len4][len5];
  printf("LCS length is %d\n", L[idx][len2][len3][len4][len5]);
}

char* findLCS_5(char **seqs, int len1, int len2, int len3, int len4, int len5, int *****L) {
  int lcsLength = L[len1 % 2][len2][len3][len4][len5];
  char *lcs = (char *)malloc(sizeof(char) * (MAX_LEN + 1));

  lcs[lcsLength] = '\0';
  int i = len1, j = len2, k = len3, l = len4, m = len5;

  // Traceback to find the LCS
  while (i > 0 && j > 0 && k > 0 && l > 0 && m > 0) {
    // If current character in X[], Y[] and Z[] are same, then current
    // character is part of LCS
    if (seqs[0][i - 1] == seqs[1][j - 1] && seqs[0][i - 1] == seqs[2][k - 1] &&
        seqs[0][i - 1] == seqs[3][l - 1] && seqs[0][i - 1] == seqs[4][m - 1]) {
      lcs[--lcsLength] = seqs[0][i - 1];  // Put current character in result
      i--, j--, k--, l--, m--;                 // Reduce values of i, j, k, l, m
      recomputeDP_5(seqs, i, len2, len3, len4, len5, L);
    } else {
      // If not same, then find the larger of two and go in the direction of
      // larger value
      if (L[(i - 1) % 2][j][k][l][m] >= L[i % 2][j - 1][k - 1][l - 1][m - 1] &&
          L[(i - 1) % 2][j][k][l][m] >= L[i % 2][j][k - 1][l][m] &&
          L[(i - 1) % 2][j][k][l][m] >= L[i % 2][j][k][l - 1][m] &&
          L[(i - 1) % 2][j][k][l][m] >= L[i % 2][j][k][l][m - 1]) {
        i--;
        recomputeDP_5(seqs, i, len2, len3, len4, len5, L);
      } else if (L[i % 2][j - 1][k][l][m] >= L[i % 2][j][k - 1][l][m] &&
                 L[i % 2][j - 1][k][l][m] >= L[i % 2][j][k][l - 1][m] &&
                 L[i % 2][j - 1][k][l][m] >= L[i % 2][j][k][l][m - 1]) {
        j--;
      } else if (L[i % 2][j][k - 1][l][m] >= L[i % 2][j][k][l - 1][m] &&
                 L[i % 2][j][k - 1][l][m] >= L[i % 2][j][k][l][m - 1]) {
        k--;
      } else if (L[i % 2][j][k][l - 1][m] >= L[i % 2][j][k][l][m - 1]) {
        l--;
      } else {
        m--;
      }
    }
  }

  // free array L
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j <= len2; j++) {
      for (int k = 0; k <= len3; k++) {
        for (int l = 0; l <= len4; l++) {
          free(L[i][j][k][l]);
        }
        free(L[i][j][k]);
      }
      free(L[i][j]);
    }
    free(L[i]);
  }

  return lcs;
}

int main() {
  char *seqs[MAX_SEQUENCES];
  int lcsLength;
  char *lcs;
  int seqs_num;

  read_input_file(seqs, &seqs_num);

  if (seqs_num == 2) {
    lcs = lcs_2(seqs);
    lcsLength = strlen(lcs);

  } else if (seqs_num == 3) {
    lcs = lcs_3(seqs);
    lcsLength = strlen(lcs);

  } else if (seqs_num == 4) {
    lcs = lcs_4(seqs);
    lcsLength = strlen(lcs);

  } else if (seqs_num == 5) {
    int *****L = (int *****)malloc(2 * sizeof(int ****));
    for (int i = 0; i < 2; i++) {
      L[i] = (int ****)malloc((MAX_LEN + 1) * sizeof(int ***));
      for (int j = 0; j <= MAX_LEN; j++) {
        L[i][j] = (int ***)malloc((MAX_LEN + 1) * sizeof(int **));
        for (int k = 0; k <= MAX_LEN; k++) {
          L[i][j][k] = (int **)malloc((MAX_LEN + 1) * sizeof(int *));
          for (int l = 0; l <= MAX_LEN; l++) {
            L[i][j][k][l] = (int *)malloc((MAX_LEN + 1) * sizeof(int));
            memset(L[i][j][k][l], 0, (MAX_LEN + 1) * sizeof(int));
          }
        }
      }
    }
    lcsLength = lcsLEN_5(seqs, strlen(seqs[0]), strlen(seqs[1]), strlen(seqs[2]), strlen(seqs[3]), strlen(seqs[4]), L);
    lcs = findLCS_5(seqs, strlen(seqs[0]), strlen(seqs[1]), strlen(seqs[2]), strlen(seqs[3]), strlen(seqs[4]), L);

  }

  printf("Length of LCS is %d\n", lcsLength);
  printf("LCS of the three strings is '%s'\n", lcs);

  MSA_write_output_file(seqs, seqs_num, lcs);

  return 0;
}