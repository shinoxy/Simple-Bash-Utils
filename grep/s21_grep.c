#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#define MAX 512

int main(int argc, char** argv) {
  if (argc > 1) {
    int files_count = 0;
    int flags_count = 0;
    struct s_info current_info = parser(argc, argv, &files_count, &flags_count);
    if (current_info.errors == 0 && files_count > 0 && current_info.template) {
      grep(current_info, files_count, flags_count);
    } else {
      printf("Error\n");
    }
    for (int i = 0; i < argc; i++) free(current_info.file_name[i]);
    free(current_info.file_name);
    free(current_info.template_char);
    free(current_info.template);
  }
  return 0;
}