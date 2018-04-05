#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int comparisons = 0;

char * frob (const char ch) {
  return (char) ch ^ 42;
}

int frobcmp (char const * a, char const * b) {
  comparisons++;
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
  char * arr;
  int buffer = 0;
  int fd = STDIN_FILENO;
  int num_words = 0;
  int i = 0;
  struct stat st;
  fstat(fd, &st);
  off_t file_size;

  // Check if file is a regular file
  if (S_ISREG(st.st_mode)) {
	file_size = st.st_size;
  }
  else {
	file_size = 500;
  }
  arr = (char*) malloc(sizeof(char) * file_size);
  if (arr == NULL) {
    fprintf(stderr, "Memory cannot be allocated.");
    exit(1);
  }

  while (read(fd, &buffer, 1) > 0) {
    // if file grows while reading, then reallocate
	if (i >= file_size) {
	  file_size = file_size * 2;
	  arr = (char*) realloc(arr, file_size);
      if (arr == NULL) {
		fprintf(stderr, "Memory cannot be re-allocated.");
		exit(1);
      }
	}
	if (buffer == '\n') {
	  break;
	}
	
	if (buffer == ' ') {
      num_words++;
	}
	
    arr[i] = (char) buffer;
    i++;
	
    
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
	  write(STDOUT_FILENO, &(*words[m]), 1);
    }
    while (*words[m] != ' ') {     
	  *words[m]++;
      write(STDOUT_FILENO, &(*words[m]), 1);
    }
	if (ferror(stdout)) {
      fprintf(stderr, "Could not write output.");
      exit(1);
    }
  }
  
  fprintf(stderr, "Comparisons: %d\n", comparisons);
  
  free(arr);
  free(words);
  exit(0);
  return 0;
}
