#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#define MAX 4096

int main(int argc, char **argv) {
  if (argc > 1) {
    int files_count = 0;
    int does_flag_exists = 0;
    struct s_info current_info =
        parser(argc, argv, &files_count, &does_flag_exists);
    if (files_count != 0) {
      concatenate(current_info, files_count, does_flag_exists);
    } else {
      fprintf(stderr, "Error\n");
    }
    for (int i = 0; i < argc; i++) {
      free(current_info.file_name[i]);
    }
    free(current_info.file_name);
    free(current_info.option);
  }
  return 0;
}

struct s_info parser(int argc, char **argv, int *files_count,
                     int *does_flag_exists) {
  struct s_info current_info = {NULL, NULL};
  current_info.file_name = malloc(sizeof(char *) * argc);
  for (int i = 0; i < argc; i++) {
    current_info.file_name[i] = malloc(sizeof(char) * 100);
  }
  current_info.option = calloc(MAX, sizeof(char));
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && *does_flag_exists == 0) {
      strcpy(current_info.option, argv[i]);
      *does_flag_exists += 1;
    } else {
      strcpy(current_info.file_name[*files_count], argv[i]);
      *files_count += 1;
    }
  }
  return current_info;
}

void concatenate(struct s_info current_info, int files_count,
                 int does_flag_exists) {
  int str_count = 0;
  for (int file_num = 0; file_num < files_count; file_num++) {
    FILE *file;
    if ((file = fopen(current_info.file_name[file_num], "r")) != NULL) {
      int lines_count = 1;
      while (!feof(file)) {
        if (fgetc(file) == '\n') {
          lines_count++;
        }
      }
      rewind(file);
      char **text = calloc(lines_count, sizeof(char *));
      for (int i = 0; i < lines_count; i++) {
        text[i] = calloc(MAX, sizeof(char));
        fgets(text[i], MAX, file);
      }
      char flag = current_info.option[1];
      char flag_gnu = current_info.option[2];
      if (flag == 's' || flag_gnu == 's') {
        option_s(text, lines_count);
      } else if (flag == 'n' ||
                 (flag_gnu == 'n' && strlen(current_info.option) == 8)) {
        option_n(text, lines_count, &str_count);
      } else if (flag == 'b' ||
                 (flag_gnu == 'n' && strlen(current_info.option) == 17)) {
        option_b(text, lines_count, &str_count);
      } else if (flag == 't') {
        option_t(text, lines_count);
      } else if (flag == 'T') {
        option_T(text, lines_count);
      } else if (flag == 'e') {
        option_e(text, lines_count);
      } else if (flag == 'E') {
        option_E(text, lines_count);
      } else if (does_flag_exists == 0) {
        for (int i = 0; i < lines_count; i++) {
          printf("%s", text[i]);
        }
      } else {
        fprintf(stderr, "Wrong flag for file: %s\n",
                current_info.file_name[file_num]);
      }
      for (int i = 0; i < lines_count; i++) {
        free(text[i]);
      }
      free(text);
      fclose(file);
    } else {
      fprintf(stderr, "No such file or directory\n");
    }
  }
}

void option_s(char **text, int lines_count) {
  for (int i = 0; i < lines_count; i++) {
    if (!(text[i][0] == '\n' && text[i + 1][0] == '\n')) {
      printf("%s", text[i]);
    }
  }
}

void option_t(char **text, int lines_count) {
  char **new_text = calloc(lines_count, sizeof(char *));
  for (int i = 0; i < lines_count; i++) {
    new_text[i] = calloc(MAX, sizeof(char));
  }
  for (int i = 0; i < lines_count; i++) {
    int new_text_count = 0;
    for (int j = 0; j < ((int)strlen(text[i]) + 1); j++) {
      if (((text[i][j] > 0 && text[i][j] < 32)) && text[i][j] != 9 &&
          text[i][j] != 10) {
        new_text[i][new_text_count] = '^';
        new_text[i][new_text_count + 1] = text[i][j] + 64;
        new_text_count += 2;
      } else if (text[i][j] == 127) {
        new_text[i][new_text_count] = '^';
        new_text[i][new_text_count + 1] = '?';
        new_text_count += 2;
      } else if (text[i][j] == 9) {
        new_text[i][new_text_count] = '^';
        new_text[i][new_text_count + 1] = 'I';
        new_text_count += 2;
      } else {
        new_text[i][new_text_count] = text[i][j];
        new_text_count++;
      }
    }
    printf("%s", new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) {
    free(new_text[i]);
  }
  free(new_text);
}

void option_e(char **text, int lines_count) {
  char **new_text = calloc(lines_count, sizeof(char *));
  for (int i = 0; i < lines_count; i++) {
    new_text[i] = calloc(MAX, sizeof(char));
  }
  for (int i = 0; i < lines_count; i++) {
    int new_text_count = 0;
    for (int j = 0; j < ((int)strlen(text[i]) + 1); j++) {
      if ((text[i][j] > 0 && text[i][j] < 32) &&
          (text[i][j] != 9 && text[i][j] != 10)) {
        new_text[i][new_text_count] = '^';
        new_text[i][new_text_count + 1] = text[i][j] + 64;
        new_text_count += 2;
      } else if (text[i][j] == 127) {
        new_text[i][new_text_count] = '^';
        new_text[i][new_text_count + 1] = '?';
        new_text_count += 2;
      } else if (text[i][j] == 10) {
        new_text[i][new_text_count] = '$';
        new_text[i][new_text_count + 1] = '\n';
        new_text[i][new_text_count + 2] = '\0';
        new_text_count += 3;
      } else {
        new_text[i][new_text_count] = text[i][j];
        new_text_count++;
      }
    }
    printf("%s", new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) {
    free(new_text[i]);
  }
  free(new_text);
}

void option_T(char **text, int lines_count) {
  char **new_text = malloc(lines_count * sizeof(char *));
  for (int i = 0; i < lines_count; i++) {
    new_text[i] = malloc(MAX * sizeof(char));
  }
  for (int i = 0; i < lines_count; i++) {
    int new_text_count = 0;
    for (int j = 0; j < (int)(strlen(text[i]) + 1); j++) {
      if (text[i][j] != '\t') {
        new_text[i][new_text_count] = text[i][j];
        new_text_count++;
      } else {
        new_text[i][new_text_count] = '^';
        new_text[i][new_text_count + 1] = 'I';
        new_text_count += 2;
      }
    }
    printf("%s", new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) {
    free(new_text[i]);
  }
  free(new_text);
}

void option_E(char **text, int lines_count) {
  char **new_text = calloc(lines_count, sizeof(char *));
  for (int i = 0; i < lines_count; i++) {
    new_text[i] = calloc(MAX, sizeof(char));
  }
  for (int i = 0; i < lines_count; i++) {
    int new_text_count = 0;
    for (int j = 0; j < (int)(strlen(text[i]) + 1); j++) {
      if (text[i][j] != '\n') {
        new_text[i][new_text_count] = text[i][j];
        new_text_count++;
      } else {
        new_text[i][new_text_count] = '$';
        new_text[i][new_text_count + 1] = '\n';
        new_text[i][new_text_count + 2] = '\0';
        new_text_count += 3;
      }
    }
    printf("%s", new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) {
    free(new_text[i]);
  }
  free(new_text);
}

void option_b(char **text, int lines_count, int *str_count) {
  for (int i = 0; i < lines_count; i++) {
    if ((text[i][0] == '\0' || text[i][0] == '\n' ||
         (text[i][0] == '$' && text[i][1] == '\n'))) {
      printf("%s", text[i]);
    } else {
      *str_count += 1;
      printf("%6d\t%s", *str_count, text[i]);
    }
  }
}

void option_n(char **text, int lines_count, int *str_count) {
  for (int i = 0; i < lines_count; i++) {
    if (text[i][0] != '\0') {
      *str_count += 1;
      printf("%6d\t%s", *str_count, text[i]);
    }
  }
}
