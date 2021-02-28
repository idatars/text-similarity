// This program computes n_gram similarity between two inputs

#include "cs136-trace.h"
#include "str-list.h"
#include "str-create-destroy.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

static const char END_OF_PARAGRAPH[] = "---";
/////////////////////////////////////////////////////////////////


// remove_punctuation(str, length) removes punctuation from the beginning
// and ending of the string
// time: O(n)
// requires: length is a valid pointer
void remove_punctuation(char str[], int *length) {
  char first_char = str[0];
  char last_char = str[*length - 1];
  while (last_char == '.' || last_char == ',' || last_char == '!' ||
         last_char == '\'' || last_char == '?' || last_char == ';' ||
         last_char == ':' || last_char == '"' || first_char == '.' ||
         first_char == ',' || first_char == '!' || first_char == '\'' ||
         first_char == '?' || first_char == ';' ||
         first_char == ':' || first_char == '"') {
    if (last_char == '.' || last_char == ',' || last_char == '!' ||
        last_char == '\'' || last_char == '?' || last_char == ';' ||
        last_char == ':' || last_char == '"') {
      str[*length - 1] = 0;
      --(*length);
      last_char = str[*length - 1];
    }
    if (first_char == '.' || first_char == ',' || first_char == '!' ||
        first_char == '\'' || first_char == '?' || first_char == ';' ||
        first_char == ':' || first_char == '"') {
      for (int i = 0; i < *length; ++i) {
        str[i] = str[i + 1];
      }
      --(*length);
      first_char = str[0];
    }
  }
}

// remove_caps(str, length) removes all capital letters from the string
// time: O(n)
void remove_caps(char str[], int length) {
  char curr;
  for (int i = 0; i < length; ++i) {
    curr = str[i];
    if (curr <= 'Z' && curr >= 'A') {
      //printf("%c\n", curr + 32);
      str[i] = curr + 32;
    }
  }
}

// read_input(s) reads input into s and applies the appropriate pre n_gram
// modifications
// time: O(n*t) where n in the number of strings and t is the length
// of the largest string
// requires: s is a valid pointer
void read_input (struct str_list *s) {
  assert(s);
  int length = 0;
  int index = 0;
  char str[1000+1];
  scanf("%s", str);
  //printf("%s\n", str);

  while (strcmp(str, END_OF_PARAGRAPH) != 0) {
    length = strlen(str);

    remove_punctuation(str, &length);
    remove_caps(str, length);
    str[length] = ' ';
    str[length+1]= 0;

    index = str_list_add(s, str);
    str_list_set_value(s, index, length);
    scanf("%s", str);
    //printf("%s\n", str);
    //str_list_print(s);
  }
}

// create_n_grams(src,dest,n_gram_len) creates all of the UNIQUE n-grams
//   of length n_gram_length that can be created from the strings
//   in the src list and adds them to dest.
//   The src list contains lower case strings/words with punctuation removed
//   (as described in the assignment specification).
//   Each string in src has length > 0 and these strings are
//   NOT required to be UNIQUE.
// effects: modifies dest
// requires: src and dest must not be null and n_gram_len > 0
// in the source, W is the length of the largest words in S, and
// N is the n gram length
void create_n_grams(struct str_list *src, struct str_list *dest,
                    int n_gram_len) {
  assert(src);
  assert(dest);
  assert(n_gram_len > 0);
  
  char n_gram[1000+1];
  int length = 0;
  int iterations = str_list_len(src) - n_gram_len;
  for (int i = 0; i <= iterations; ++i) {
    for (int j = 0; j < n_gram_len; ++j) {
      strcat(n_gram, str_list_get_string(src, j + i));
    }
    str_list_add(dest, n_gram);
    n_gram[0] = 0;
    length = 0;
  }
}


// unique_n_grams(s, t, k1, k2) returns the number of elements of s that
// are also in t. It returns by pointers the number of unique elements
// of each by k1 and k2
// requires: s, t, k1, k2 are valid pointers
// of elements in T
int unique_n_grams(struct str_list *s, struct str_list *t,
                   int *k1, int *k2) {
  assert(s);
  assert(t);
  assert(k1);
  assert(k2);
  struct str_list *sofar = str_list_create();
  char n_gram[1000+1];
  int C = 0;

  for (int i = 0; i < str_list_len(s); ++i) {
    strcpy(n_gram, str_list_get_string(s, i));
    if (str_list_find(sofar, n_gram) < 0) {
      ++(*k1);
      if (str_list_find(t, n_gram) >= 0) {
        ++C;
      }
    }
  }

  str_list_reset(sofar);

  for (int j = 0; j < str_list_len(t); ++j) {
    strcpy(n_gram, str_list_get_string(t, j));
    if (str_list_find(sofar, n_gram) < 0) {
      ++(*k2);
    }
  }

  str_list_destroy(sofar);
  return C;
}

// min(a, b) returns the lower integer
int min(int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

int main(void) {
  struct str_list *s = str_list_create();
  struct str_list *t = str_list_create();
  struct str_list *s_n_grams = str_list_create();
  struct str_list *t_n_grams = str_list_create();
  int n_gram_len = 0;
  int k1 = 0;
  int k2 = 0;
  int k = 0;
  int C = 0;
  int score = 0;

  read_input(s);
  read_input(t);

  while (scanf("%d", &n_gram_len) == 1) {
    create_n_grams(s, s_n_grams, n_gram_len);
    create_n_grams(t, t_n_grams, n_gram_len);

    k1 = 0;
    k2 = 0;
    score = 0;
    C = unique_n_grams(s_n_grams, t_n_grams, &k1, &k2);
    k = min(k1, k2);
    if (k != 0) {
      score = (C * 100) / k;
    }
    printf("Similarity score using n-grams of size %d = %d\n",
           n_gram_len, score);
    
    str_list_reset(s_n_grams);
    str_list_reset(t_n_grams);
  }
  str_list_destroy(s);
  str_list_destroy(t);
  str_list_destroy(s_n_grams);
  str_list_destroy(t_n_grams);
}
