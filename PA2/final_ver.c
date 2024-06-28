#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SEQUENCES 5
#define MAX_STR_LENGTH 130  // 120 + 10(for safety)

// Math Helper Functions=======================================
// Return the sum of the given integer array
int sum(int *arr, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  return sum;
}

// Return the maximum value of the given integer array
int max(int *arr, int size) {
  int max = arr[0];
  for (int i = 1; i < size; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

// Return the index of the maximum value of the given integer array
int max_index(int *arr, int size) {
  int max = arr[0];
  int max_idx = 0;
  for (int i = 1; i < size; i++) {
    if (arr[i] > max) {
      max = arr[i];
      max_idx = i;
    }
  }
  return max_idx;
}

// LCS Function Helper Structures=======================================
// node structure for memoization table
typedef struct node {
  int name[MAX_SEQUENCES];
  int lcs_len;
  struct node *next_memo;
  int chosen_name[MAX_SEQUENCES];
} node;

// LCS HELPER FUNCTIONS=======================================
// 0 : contains -1,
// 1 : does not contain -1
int check_arr(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == -1) {
      return 0;
    }
  }
  return 1;
}

// Dynamic Programming for recursively calculating the lcs length using
// memoization table (2 to 4 sequences)
int calc_lcs_len_2t4(int seqs_num,
                     int seqs_char_idx_arr[MAX_SEQUENCES][4][MAX_STR_LENGTH],
                     node *****memo_table, int *name) {
  // if node name contains -1
  if (!check_arr(name, seqs_num)) {
    return 0;
  } else {
    // if the name is in the memo table
    node *finder = memo_table[name[0]][name[1]][name[2]][name[3]];
    if (finder != NULL) {
      return finder->lcs_len;
    }
    // if the name is not in the memo table
    else {
      node *new_node = malloc(sizeof(node));
      for (int i = 0; i < MAX_SEQUENCES; i++) {
        if (i < seqs_num) {
          new_node->name[i] = name[i];
        } else {
          new_node->name[i] = 0;
        }
      }
      memo_table[name[0]][name[1]][name[2]][name[3]] = new_node;

      // check the next node
      int next_name[4][MAX_SEQUENCES];
      for (int i = 0; i < MAX_SEQUENCES; i++) {
        if (i < seqs_num) {
          next_name[0][i] = seqs_char_idx_arr[i][0][name[i]];
          next_name[1][i] = seqs_char_idx_arr[i][1][name[i]];
          next_name[2][i] = seqs_char_idx_arr[i][2][name[i]];
          next_name[3][i] = seqs_char_idx_arr[i][3][name[i]];
        } else {
          next_name[0][i] = 0;
          next_name[1][i] = 0;
          next_name[2][i] = 0;
          next_name[3][i] = 0;
        }
      }

      // calculate the lcs length
      int temp_max[4];
      for (int i = 0; i < 4; i++) {
        temp_max[i] = calc_lcs_len_2t4(seqs_num, seqs_char_idx_arr, memo_table,
                                       next_name[i]);
      }

      // check the max of the lcs length and store the chosen name,lcs_len
      int max_idx = max_index(temp_max, 4);
      for (int i = 0; i < MAX_SEQUENCES; i++) {
        if (i < seqs_num) {
          new_node->chosen_name[i] = next_name[max_idx][i];
        } else {
          new_node->chosen_name[i] = 0;
        }
      }
      new_node->lcs_len = temp_max[max_idx] + 1;
      return max(temp_max, 4) + 1;
    }
  }
}

// Dynamic Programming for recursively calculating the lcs length using
// memoization table (5 sequences)
int calc_lcs_len_5(int seqs_num,
                   int seqs_char_idx_arr[MAX_SEQUENCES][4][MAX_STR_LENGTH],
                   node *****memo_table, int *name) {
  // if node name contains -1
  if (!check_arr(name, seqs_num)) {
    return 0;
  } else {
    // check if the name is in the memo table
    int match = 0;
    node *finder = memo_table[name[0]][name[1]][name[2]][name[3]];
    while (finder != NULL) {
      if (finder->name[4] == name[4]) {
        match = 1;
        break;
      }
      if (finder->next_memo == NULL) {
        break;
      }
      finder = finder->next_memo;
    }
    // if the name is in the memo table
    if (match) {
      return finder->lcs_len;
    }
    // if the name is not in the memo table
    else {
      node *new_node = malloc(sizeof(node));
      for (int i = 0; i < seqs_num; i++) {
        new_node->name[i] = name[i];
      }
      if (finder == NULL) {
        memo_table[name[0]][name[1]][name[2]][name[3]] = new_node;
      } else {
        finder->next_memo = new_node;
      }

      // check the next node
      int next_name[4][MAX_SEQUENCES];
      for (int i = 0; i < seqs_num; i++) {
        next_name[0][i] = seqs_char_idx_arr[i][0][name[i]];
        next_name[1][i] = seqs_char_idx_arr[i][1][name[i]];
        next_name[2][i] = seqs_char_idx_arr[i][2][name[i]];
        next_name[3][i] = seqs_char_idx_arr[i][3][name[i]];
      }

      // calculate the lcs length
      int temp_max[4];
      for (int i = 0; i < 4; i++) {
        temp_max[i] = calc_lcs_len_5(seqs_num, seqs_char_idx_arr, memo_table,
                                     next_name[i]);
      }

      // check the max of the lcs length and store the chosen name,lcs_len
      int max_idx = max_index(temp_max, 4);
      for (int i = 0; i < seqs_num; i++) {
        new_node->chosen_name[i] = next_name[max_idx][i];
      }
      new_node->lcs_len = temp_max[max_idx] + 1;
      return max(temp_max, 4) + 1;
    }
  }
}

// function for get the actual lcs (2 to 4 sequences)
char *get_lcs_2t4(char **seqs, int seqs_num, node *current, int lcs_len,
                  node *****memo_table) {
  // initialize the lcs
  char *lcs = malloc(sizeof(char) * (lcs_len + 1));
  lcs[lcs_len] = '\0';
  // match the chosen name to memo table node & find the lcs
  node *finder;
  for (int i = 0; i < lcs_len; i++) {
    finder = memo_table[current->chosen_name[0]][current->chosen_name[1]]
                       [current->chosen_name[2]][current->chosen_name[3]];
    lcs[i] = seqs[0][current->chosen_name[0] - 1];
    current = finder;
  }
  printf("%s\n", lcs);
  return lcs;
}

// function for get the actual lcs (5 sequences)
char *get_lcs_5(char **seqs, int seqs_num, node *current, int lcs_len,
                node *****memo_table) {
  // initialize the lcs
  char *lcs = malloc(sizeof(char) * (lcs_len + 1));
  lcs[lcs_len] = '\0';
  // match the chosen name to memo table node & find the lcs
  node *finder;
  for (int i = 0; i < lcs_len; i++) {
    int match = 0;
    finder = memo_table[current->chosen_name[0]][current->chosen_name[1]]
                       [current->chosen_name[2]][current->chosen_name[3]];
    while (finder != NULL) {
      if (finder->name[4] == current->chosen_name[4]) {
        match = 1;
        break;
      }
      if (finder->next_memo == NULL) {
        break;
      }
      finder = finder->next_memo;
    }
    if (!match) {
      break;
    }
    lcs[i] = seqs[0][current->chosen_name[0] - 1];
    current = finder;
  }
  printf("%s\n", lcs);
  return lcs;
}

// LCS FUNCTIONS=======================================
// function for find the lcs of the given sequences
char *find_lcs(char **seqs, int seqs_num) {
  // build seqs_char_idx_arr to check the next index of each character
  int seqs_char_idx_arr[MAX_SEQUENCES][4][MAX_STR_LENGTH];
  memset(seqs_char_idx_arr, -1, sizeof(seqs_char_idx_arr));
  int start_idx_A, start_idx_T, start_idx_G, start_idx_C;

  // fill in the seqs_char_idx_arr
  for (int i = 0; i < seqs_num; i++) {
    start_idx_A = 0, start_idx_T = 0, start_idx_G = 0, start_idx_C = 0;
    for (int k = 0; k < strlen(seqs[i]); k++) {
      if (seqs[i][k] == 'A') {
        for (int idx = start_idx_A; idx <= k; idx++) {
          seqs_char_idx_arr[i][0][idx] = k + 1;
        }
        start_idx_A = k + 1;
      } else if (seqs[i][k] == 'T') {
        for (int idx = start_idx_T; idx <= k; idx++) {
          seqs_char_idx_arr[i][1][idx] = k + 1;
        }
        start_idx_T = k + 1;
      } else if (seqs[i][k] == 'G') {
        for (int idx = start_idx_G; idx <= k; idx++) {
          seqs_char_idx_arr[i][2][idx] = k + 1;
        }
        start_idx_G = k + 1;
      } else if (seqs[i][k] == 'C') {
        for (int idx = start_idx_C; idx <= k; idx++) {
          seqs_char_idx_arr[i][3][idx] = k + 1;
        }
        start_idx_C = k + 1;
      }
    }
  }

  // malloc memoization table of 4D array whose element is the pointer of node
  node *****memo_table = malloc(sizeof(node ****) * (MAX_STR_LENGTH));
  for (int i = 0; i < MAX_STR_LENGTH; i++) {
    memo_table[i] = malloc(sizeof(node ***) * (MAX_STR_LENGTH));
    for (int j = 0; j < MAX_STR_LENGTH; j++) {
      memo_table[i][j] = malloc(sizeof(node **) * (MAX_STR_LENGTH));
      for (int k = 0; k < MAX_STR_LENGTH; k++) {
        memo_table[i][j][k] = malloc(sizeof(node *) * (MAX_STR_LENGTH));
      }
    }
  }
  // initialize the root node & declare the lcs
  node *root = malloc(sizeof(node));
  for (int i = 0; i < MAX_SEQUENCES; i++) {
    root->name[i] = 0;
  }
  char *lcs;

  // calculate the lcs length & get the actual lcs
  // for 5 sequences
  if (seqs_num == 5) {
    int lcs_len =
        calc_lcs_len_5(seqs_num, seqs_char_idx_arr, memo_table, root->name) - 1;
    node *current = memo_table[0][0][0][0];  // root node
    printf("lcs_len: %d\n", lcs_len);
    lcs = get_lcs_5(seqs, seqs_num, current, lcs_len, memo_table);
  }
  // for 2 to 4 sequences
  else {
    int lcs_len =
        calc_lcs_len_2t4(seqs_num, seqs_char_idx_arr, memo_table, root->name) -
        1;
    node *current = memo_table[0][0][0][0];  // root node
    printf("lcs_len: %d\n", lcs_len);
    lcs = get_lcs_2t4(seqs, seqs_num, current, lcs_len, memo_table);
  }
  return lcs;
}

// READ & WRITE FILE FUNCTIONS=======================================
// Read the input file and store the sequences in seqs
void read_input_file(char **seqs, int *seqs_num) {
  FILE *file = fopen("hw2_input.txt", "r");
  // get the first line as seqs_num
  fscanf(file, "%d", seqs_num);
  // ignore the second line with dollar sign
  fscanf(file, "%*s");
  // read the sequences from 3rd line, and store them in seqs
  for (int i = 0; i < *seqs_num; i++) {
    seqs[i] = malloc(sizeof(char) * MAX_STR_LENGTH);
    fscanf(file, "%s", seqs[i]);
  }
}

// function for MSA and write the output file
void MSA_write_output_file(char **seqs, int seqs_num, char *lcs) {
  int match[seqs_num];
  char aligned_seqs[seqs_num][MAX_STR_LENGTH * seqs_num + 1];
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
    // Check match to lcs or not
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

  // Find the maximum length of the aligned sequences to add null character
  int max_alginment_length = max(aligned_idx, seqs_num);
  for (int i = 0; i < seqs_num; i++) {
    aligned_idx[i] = max_alginment_length;
    aligned_seqs[i][max_alginment_length] = '\0';
  }

  // write the output file of the aligned sequences
  FILE *file = fopen("hw2_output.txt", "w");
  for (int i = 0; i < seqs_num; i++) {
    fprintf(file, "%s\n", aligned_seqs[i]);
  }

  // Write "*" for the matched positions in the last line
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

// main function =======================================
int main() {
  // Checking run time at beginning of main
  int TIME = 0;
  clock_t start = clock();

  char *seqs[MAX_SEQUENCES];                   // array of sequences
  int seqs_num;                                // number of sequences
  read_input_file(seqs, &seqs_num);            // read the input file
  char *lcs = find_lcs(seqs, seqs_num);        // find the lcs
  MSA_write_output_file(seqs, seqs_num, lcs);  // MSA and write the output file

  // at end of main
  TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
  printf("TIME : %d ms\n", TIME);
  return 0;
}
