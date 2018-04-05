#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of inputs.");
    exit(1);
  }
  char * str1;
  char * str2;

  str1 = argv[1];
  if (str1 == NULL) {
    fprintf(stderr, "String could not be created.");
    exit(1);
  }

  str2 = argv[2];
  if (str2 == NULL) {
    fprintf(stderr, "String could not be created.");
    exit(1);
  }

  int len1 = strlen(str1);
  int len2 = strlen(str2);

  int i = 0;
  int j = 0;

  // Check for duplicate bytes
  for (i = 0; i < len1 - 1; i++) {
    for (j = i + 1; j < len1; j++) {
      if (str1[i] == str1[j]) {
        fprintf(stderr, "Duplicate bytes!\n");
        exit(1);
      }
    }
  }

  // Check for unequal lengths
  if (len1 != len2) {
    fprintf(stderr, "Unequal lengths!\n");
    exit(1);
  }

  char buffer = 'a';
  int fd = STDIN_FILENO;
  ssize_t reader = read(fd, &buffer, 1);
  int k = 0;

  while (reader > 0) {
    for (k = 0; k < len1; k++) {
      if (buffer == str1[k]) {
		buffer = str2[k];
		break;
      }
    } 

    write(STDOUT_FILENO, &buffer, 1);    
    reader = read(fd, &buffer, 1);
  }
  return 0;
}
