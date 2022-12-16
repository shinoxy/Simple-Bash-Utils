#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#define MAX 512

int main(int argc, char** argv) {
  if (argc > 1) {
    int files_count = 0;
    struct s_info current_info = parser(argc, argv, &files_count);
    if (current_info.errors == 0 && files_count != 0) {
      int is_not_first_file = 0;
      int str_counter = 0;
      concatenate(current_info, files_count, &str_counter, &is_not_first_file);
    } else {
      printf("Error\n");
    }
    for (int i = 0; i < argc; i++) {  // Освобождение памяти
      free(current_info.file_name[i]);
    }
    free(current_info.file_name);
  } else {
    while (1) {
      char str[MAX];
      scanf("%s", str);
      printf("%s", str);
    }
  }
  return 0;
}