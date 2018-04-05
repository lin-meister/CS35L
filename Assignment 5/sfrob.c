#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char * frob (const char ch) {
  return (char) ch ^ 42;
}

int frobcmp (char const * a, char const * b) {
  char *ch1 = *(char **) a;
  char *ch2 = *(char **) b;
  
  while (*ch1 != ' ' && *ch2 != ' ') {
    if (frob(*ch1) < frob(*ch2)) {
      return -1;
    }
    else if (frob(*ch1) > frob(*ch2)) {
      return 1;
    }
    ch1++;
    ch2++;
  }

  if (*ch1 == ' ' && *ch2 != ' ') {
    return -1;
  }
  else if (*ch1 != ' ' && *ch2 == ' ') {
    return 1;
  }
  return 0;
}

int main (int argc, const char *argv[])
{
  int size = 500;
  char * arr;
  arr = (char*) malloc(sizeof(char) * size);
  if (arr == NULL) {
    fprintf(stderr, "Memory cannot be allocated.");
    exit(1);
  }

  int input = 0;
  int i = 0;
  int num_words = 0;

  while (input != EOF) {
    // if input size greater than 500, then reallocate
    if (input >= size) {
      arr = (char*) realloc(arr, size*2);
      if (arr == NULL) {
		fprintf(stderr, "Memory cannot be re-allocated.");
		exit(1);
      }
    }

    input = getchar();
   
    if (input != EOF) {
      if (input == '\n') {
		break;
      }
      if (input == ' ') {
		num_words++;
      }
      arr[i] = (char) input;
      i++;
    }

    if (ferror(stdin)) {
      fprintf(stderr, "Could not read input.");
    }

  }

  if (arr[i] != ' ') {
    num_words++;
    strcat(arr, " ");
  }
  
  
  // Allocate space for an array for the words
  char ** words;
  words = (char**)malloc(sizeof(char*) * (num_words));
  if (words == NULL) {
    fprintf(stderr, "Memory cannot be allocated.");
    exit(1);
  }

  // Iterate through the input, and store the pointers to the individual words
  // Separate them based on space
  char * arr_ptr = arr;
  int j = 0;
  int k = 0;
  for (j = 0; j < num_words; ++j) {
    words[j] = arr_ptr;
    while (*arr_ptr != ' ') {
      arr_ptr++;
    }
    arr_ptr++;
  }
  
  qsort(words, num_words, sizeof(char*), frobcmp);
  
  int m = 0;
  // Print the sorted words
  for (m = 0; m < num_words; ++m) {
    if (*words[m] != ' ') {
      putchar(*words[m]);
    }
    while (*words[m] != ' ') {
      *words[m]++;
      putchar(*words[m]);
    }
  }
  if (ferror(stdout)) {
    fprintf(stderr, "Could not write output.");
    exit(1);
  }
  free(arr);
  free(words);
  exit(0);
  return 0;
}
