#include "functions.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#define MAX 512

struct s_info parser(int argc, char** argv, int* files_count) {
  struct s_info current_info = {NULL, 0, 0, 0, 0, 0, 0, 0, 0};
  char** flags_arr = malloc(sizeof(char*) * argc);  // Массив флагов
  for (int i = 0; i < argc; i++) flags_arr[i] = malloc(sizeof(char) * 100);
  int flags_count = 0;
  current_info.file_name = malloc(sizeof(char*) * argc);  // Массив файлов
  for (int i = 0; i < argc; i++)
    current_info.file_name[i] = malloc(sizeof(char) * 100);
  for (int i = 1; i < argc; i++) {  // Распределение на флаги и файлы
    if (argv[i][0] == '-') {
      strcpy(flags_arr[flags_count], argv[i]);
      flags_count++;
    } else {
      strcpy(current_info.file_name[*files_count], argv[i]);
      *files_count += 1;
    }
  }
  flags_processing(&current_info, flags_arr, flags_count);  // Обработка флагов
  for (int i = 0; i < argc; i++) {  // Освобождение памяти
    free(flags_arr[i]);
  }
  free(flags_arr);
  return current_info;
}

void flags_processing(struct s_info* current_info, char** flags_arr,
                      int flags_count) {
  for (int i = 0; i < flags_count; i++) {  // Обработка флагов
    if (flags_arr[i][1] == '-') {
      if (strcmp(flags_arr[i], "--number-nonblank") == 0)
        current_info->opt_b = 1;
      else if (strcmp(flags_arr[i], "--number") == 0)
        current_info->opt_n = 1;
      else if (strcmp(flags_arr[i], "--squeeze-blank") == 0)
        current_info->opt_s = 1;
      else
        current_info->errors = 1;
    } else {
      for (int j = 1; j < (int)strlen(flags_arr[i]); j++) {
        switch (flags_arr[i][j]) {
          case 'b':
            current_info->opt_b = 1;
            break;
          case 'n':
            current_info->opt_n = 1;
            break;
          case 's':
            current_info->opt_s = 1;
            break;
          case 'e':
            current_info->opt_e = 1;
            break;
          case 't':
            current_info->opt_t = 1;
            break;
          case 'E':
            current_info->opt_E = 1;
            break;
          case 'T':
            current_info->opt_T = 1;
            break;
          default:
            current_info->errors = 1;
            break;
        }
      }
    }
  }
}

void concatenate(struct s_info current_info, int files_count, int* str_counter,
                 int* is_not_first_file) {
  for (int i = 0; i < files_count; i++) {
    FILE* file;
    if ((file = fopen(current_info.file_name[i], "r")) != NULL) {
      int lines_count = 1;  // Подсчет строк в файле
      while (!feof(file))
        if (fgetc(file) == '\n') lines_count++;
      rewind(file);
      char** text =
          calloc(lines_count, sizeof(char*));  // Массив строк из файла
      for (int i = 0; i < lines_count; i++) {
        text[i] = calloc(MAX, sizeof(char));
        fgets(text[i], MAX, file);
      }
      if (current_info.opt_s == 1) option_s(text, lines_count);
      if (current_info.opt_t == 1) option_t(text, lines_count);
      if (current_info.opt_e == 1) option_e(text, lines_count);
      if (current_info.opt_T == 1 && current_info.opt_t == 0)
        option_T(text, lines_count);
      if (current_info.opt_E == 1 && current_info.opt_e == 0)
        option_E(text, lines_count);
      print_text(current_info, text, lines_count, str_counter,
                 *is_not_first_file);
      *is_not_first_file = 1;
      for (int i = 0; i < lines_count; i++)  // Освобождение памяти
        free(text[i]);
      free(text);
      fclose(file);
    } else {
      printf("No such file or directory\n");
    }
  }
}

void option_s(char** text, int lines_count) {
  char** new_text = calloc(lines_count, sizeof(char*));
  for (int i = 0; i < lines_count; i++) new_text[i] = calloc(MAX, sizeof(char));
  int new_lines_count = 0;
  for (int i = 0; i < lines_count; i++) {
    if (!(text[i][0] == '\n' && text[i + 1][0] == '\n')) {
      strcpy(new_text[new_lines_count], text[i]);
      new_lines_count++;
    }
  }
  for (int i = 0; i < lines_count; i++) strcpy(text[i], new_text[i]);
  for (int i = 0; i < lines_count; i++) free(new_text[i]);
  free(new_text);
}

void option_t(char** text, int lines_count) {
  char** new_text = calloc(lines_count, sizeof(char*));
  for (int i = 0; i < lines_count; i++) new_text[i] = calloc(MAX, sizeof(char));
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
    strcpy(text[i], new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) free(new_text[i]);
  free(new_text);
}

void option_e(char** text, int lines_count) {
  char** new_text = calloc(lines_count, sizeof(char*));
  for (int i = 0; i < lines_count; i++) new_text[i] = calloc(MAX, sizeof(char));

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
    strcpy(text[i], new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) free(new_text[i]);
  free(new_text);
}

void option_T(char** text, int lines_count) {
  char** new_text = malloc(lines_count * sizeof(char*));
  for (int i = 0; i < lines_count; i++)
    new_text[i] = malloc(MAX * sizeof(char));
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
    strcpy(text[i], new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) free(new_text[i]);
  free(new_text);
}

void option_E(char** text, int lines_count) {
  char** new_text = calloc(lines_count, sizeof(char*));
  for (int i = 0; i < lines_count; i++) new_text[i] = calloc(MAX, sizeof(char));
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
    strcpy(text[i], new_text[i]);
  }
  for (int i = 0; i < lines_count; i++) free(new_text[i]);
  free(new_text);
}

void print_text(struct s_info current_info, char** text, int lines_count,
                int* str_counter, int is_not_first_file) {
  if (current_info.opt_b == 1) {  // Нумерует только непустые строки
    if (is_not_first_file == 1) printf("%s", text[0]);
    for (int i = is_not_first_file; i < lines_count; i++) {
      if ((text[i][0] == '\0' || text[i][0] == '\n' ||
           (text[i][0] == '$' && text[i][1] == '\n'))) {
        printf("%s", text[i]);
      } else {
        if (*str_counter < 10000000) {
          *str_counter += 1;
          printf("%6d\t%s", *str_counter, text[i]);
        } else {
          *str_counter += 1;
          printf("%6d\t\t%s", *str_counter, text[i]);
        }
      }
    }
  } else if (current_info.opt_n == 1 &&
             current_info.opt_b == 0) {  // Нумерует все строки
    if (is_not_first_file == 1) printf("%s", text[0]);
    for (int i = is_not_first_file; i < lines_count - 1; i++) {
      if (*str_counter < 10000000) {
        *str_counter += 1;
        printf("%6d\t%s", *str_counter, text[i]);
      } else {
        *str_counter += 1;
        printf("%6d\t\t%s", *str_counter, text[i]);
      }
    }
  } else {  // Вывод без нумерации
    for (int i = 0; i < lines_count; i++) printf("%s", text[i]);
  }
}