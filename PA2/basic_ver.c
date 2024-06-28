#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 120  // maximum length of a sequence is 120

int max(int a, int b) {
  if (a >= b) {
    return a;
  }
  return b;
}

// function for finding the length of LCS of 2 sequences
// based on Algorithm Lecture Slide (pseudo code for LCS Length Algorithm, How
// to find actual LCS)
char *lcs_2(char *sequence1, char *sequence2) {
  int len1 = strlen(sequence1);
  int len2 = strlen(sequence2);

  int **matrix = (int **)malloc((len1 + 1) * sizeof(int *));
  for (int i = 0; i <= len1; i++) {
    matrix[i] = (int *)malloc((len2 + 1) * sizeof(int));
  }

  for (int idx1 = 0; idx1 <= len1; idx1++) {
    for (int idx2 = 0; idx2 <= len2; idx2++) {
      if (idx1 == 0 || idx2 == 0) {
        matrix[idx1][idx2] = 0;
      } else if (sequence1[idx1 - 1] == sequence2[idx2 - 1]) {
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
  char *subsequence = (char *)malloc((length + 1) * sizeof(char));
  subsequence[length] = '\0';

  while (i > 0 && j > 0) {
    if ((sequence1[i - 1] == sequence2[j - 1]) &&
        (matrix[i][j] == matrix[i - 1][j - 1] + 1)) {
      subsequence[length - 1] =
          sequence1[i - 1];  // Corrected sequence assignment
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

  return subsequence;
}

// function for finding the length of LCS of 3 sequences
// based on Algorithm Lecture Slide (pseudo code for LCS Length Algorithm, How
// to find actual LCS)
char *lcs_3(char *sequence1, char *sequence2, char *sequence3) {
  int len1 = strlen(sequence1);
  int len2 = strlen(sequence2);
  int len3 = strlen(sequence3);

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
        } else if (sequence1[idx1 - 1] == sequence2[idx2 - 1] &&
                   sequence1[idx1 - 1] == sequence3[idx3 - 1]) {
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
  char *subsequence = (char *)malloc((length + 1) * sizeof(char));
  subsequence[length] = '\0';

  while (i > 0 && j > 0 && k > 0) {
    if ((matrix[i][j][k] == matrix[i - 1][j - 1][k - 1] + 1) &&
        (sequence1[i - 1] == sequence2[j - 1]) &&
        (sequence1[i - 1] == sequence3[k - 1])) {
      subsequence[length - 1] = sequence1[i - 1];
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

  return subsequence;
}

// function for finding the length of LCS of 4 sequences
// based on Algorithm Lecture Slide (pseudo code for LCS Length Algorithm, How
// to find actual LCS)
char *lcs_4(char *sequence1, char *sequence2, char *sequence3,
            char *sequence4) {
  int len1 = strlen(sequence1);
  int len2 = strlen(sequence2);
  int len3 = strlen(sequence3);
  int len4 = strlen(sequence4);

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
          } else if (sequence1[idx1 - 1] == sequence2[idx2 - 1] &&
                     sequence1[idx1 - 1] == sequence3[idx3 - 1] &&
                     sequence1[idx1 - 1] == sequence4[idx4 - 1]) {
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
  char *subsequence = (char *)malloc((length + 1) * sizeof(char));
  subsequence[length] = '\0';

  while (i > 0 && j > 0 && k > 0 && l > 0) {
    if ((matrix[i][j][k][l] == matrix[i - 1][j - 1][k - 1][l - 1] + 1) &&
        (sequence1[i - 1] == sequence2[j - 1]) &&
        (sequence1[i - 1] == sequence3[k - 1]) &&
        (sequence1[i - 1] == sequence4[l - 1])) {
      subsequence[length - 1] = sequence1[i - 1];
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

  return subsequence;
}

// function for finding the length of LCS of 5 sequences
// based on Algorithm Lecture Slide (pseudo code for LCS Length Algorithm, How
// to find actual LCS)
char *lcs_5(char *sequence1, char *sequence2, char *sequence3, char *sequence4,
            char *sequence5) {
  int len1 = strlen(sequence1);
  int len2 = strlen(sequence2);
  int len3 = strlen(sequence3);
  int len4 = strlen(sequence4);
  int len5 = strlen(sequence5);

  int *****matrix = (int *****)malloc((len1 + 1) * sizeof(int ****));
  for (int i = 0; i <= len1; i++) {
    matrix[i] = (int ****)malloc((len2 + 1) * sizeof(int ***));
    for (int j = 0; j <= len2; j++) {
      matrix[i][j] = (int ***)malloc((len3 + 1) * sizeof(int **));
      for (int k = 0; k <= len3; k++) {
        matrix[i][j][k] = (int **)malloc((len4 + 1) * sizeof(int *));
        for (int l = 0; l <= len4; l++) {
          matrix[i][j][k][l] = (int *)malloc((len5 + 1) * sizeof(int));
        }
      }
    }
  }

  for (int idx1 = 0; idx1 <= len1; idx1++) {
    for (int idx2 = 0; idx2 <= len2; idx2++) {
      for (int idx3 = 0; idx3 <= len3; idx3++) {
        for (int idx4 = 0; idx4 <= len4; idx4++) {
          for (int idx5 = 0; idx5 <= len5; idx5++) {
            if (idx1 == 0 || idx2 == 0 || idx3 == 0 || idx4 == 0 || idx5 == 0) {
              matrix[idx1][idx2][idx3][idx4][idx5] = 0;
            } else if (sequence1[idx1 - 1] == sequence2[idx2 - 1] &&
                       sequence1[idx1 - 1] == sequence3[idx3 - 1] &&
                       sequence1[idx1 - 1] == sequence4[idx4 - 1] &&
                       sequence1[idx1 - 1] == sequence5[idx5 - 1]) {
              matrix[idx1][idx2][idx3][idx4][idx5] =
                  matrix[idx1 - 1][idx2 - 1][idx3 - 1][idx4 - 1][idx5 - 1] + 1;
            } else {
              matrix[idx1][idx2][idx3][idx4][idx5] =
                  max(max(max(max(matrix[idx1 - 1][idx2][idx3][idx4][idx5],
                                  matrix[idx1][idx2 - 1][idx3][idx4][idx5]),
                              matrix[idx1][idx2][idx3 - 1][idx4][idx5]),
                          matrix[idx1][idx2][idx3][idx4 - 1][idx5]),
                      matrix[idx1][idx2][idx3][idx4][idx5 - 1]);
            }
          }
        }
      }
    }
  }

  int i = len1;
  int j = len2;
  int k = len3;
  int l = len4;
  int m = len5;
  int length = matrix[i][j][k][l][m];
  char *subsequence = (char *)malloc((length + 1) * sizeof(char));
  subsequence[length] = '\0';

  while (i > 0 && j > 0 && k > 0 && l > 0 && m > 0) {
    if ((matrix[i][j][k][l][m] ==
         matrix[i - 1][j - 1][k - 1][l - 1][m - 1] + 1) &&
        (sequence1[i - 1] == sequence2[j - 1]) &&
        (sequence1[i - 1] == sequence3[k - 1]) &&
        (sequence1[i - 1] == sequence4[l - 1]) &&
        (sequence1[i - 1] == sequence5[m - 1])) {
      subsequence[length - 1] = sequence1[i - 1];
      i--;
      j--;
      k--;
      l--;
      m--;
      length--;
    } else if (matrix[i - 1][j][k][l][m] >= matrix[i][j - 1][k][l][m] &&
               matrix[i - 1][j][k][l][m] >= matrix[i][j][k - 1][l][m] &&
               matrix[i - 1][j][k][l][m] >= matrix[i][j][k][l - 1][m] &&
               matrix[i - 1][j][k][l][m] >= matrix[i][j][k][l][m - 1]) {
      i--;
    } else if (matrix[i][j - 1][k][l][m] >= matrix[i - 1][j][k][l][m] &&
               matrix[i][j - 1][k][l][m] >= matrix[i][j][k - 1][l][m] &&
               matrix[i][j - 1][k][l][m] >= matrix[i][j][k][l - 1][m] &&
               matrix[i][j - 1][k][l][m] >= matrix[i][j][k][l][m - 1]) {
      j--;
    } else if (matrix[i][j][k - 1][l][m] >= matrix[i - 1][j][k][l][m] &&
               matrix[i][j][k - 1][l][m] >= matrix[i][j - 1][k][l][m] &&
               matrix[i][j][k - 1][l][m] >= matrix[i][j][k][l - 1][m] &&
               matrix[i][j][k - 1][l][m] >= matrix[i][j][k][l][m - 1]) {
      k--;
    } else if (matrix[i][j][k][l - 1][m] >= matrix[i - 1][j][k][l][m] &&
               matrix[i][j][k][l - 1][m] >= matrix[i][j - 1][k][l][m] &&
               matrix[i][j][k][l - 1][m] >= matrix[i][j][k - 1][l][m] &&
               matrix[i][j][k][l - 1][m] >= matrix[i][j][k][l][m - 1]) {
      l--;
    } else {
      m--;
    }
  }

  // Free the dynamically allocated matrix memory
  for (int i = 0; i <= len1; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return subsequence;
}

int main() {
  char *sequences[] = {"ATGGAATCCATTTTAGGATTTCCAT", "AAAGGCCATTGGATCCAT", "ATTTAGCCGCCAAATGTTCAG"};
  char *lcs = lcs_3(sequences[0], sequences[1], sequences[2]);
  printf("LCS : %s\n", lcs);
  return 0;
}
