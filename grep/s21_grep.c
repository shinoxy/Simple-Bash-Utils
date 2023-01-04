#include <dirent.h>
#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#define MAX 4096

int main(int argc, char **argv) {
  if (argc > 2) {
    int files_count = 0;
    int flags_count = 0;
    int e_status = 0;
    struct s_info current_info =
        parser(argc, argv, &files_count, &flags_count, e_status);
    if (current_info.errors == 0 && files_count > 0 && current_info.template) {
      grep(current_info, files_count, flags_count, e_status);
    } else {
      fprintf(stderr, "Error\n");
    }
    for (int i = 0; i < argc; i++) {
      free(current_info.file_name[i]);
    }
    free(current_info.file_name);
    free(current_info.template_char);
    free(current_info.template);
  } else {
    fprintf(stderr, "Error\n");
  }
  return 0;
}

struct s_info parser(int argc, char **argv, int *files_count, int *flags_count,
                     int e_status) {
  struct s_info current_info = {NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  current_info.template_char = malloc(sizeof(char *) * MAX);
  current_info.file_name = malloc(sizeof(char *) * argc);
  for (int i = 0; i < argc; i++) {
    current_info.file_name[i] = malloc(sizeof(char) * 255);
  }
  int templates_count = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'e' && strlen(argv[i]) > 2) {
        e_status++;
        strcpy(current_info.template_char, &argv[i][2]);
        templates_count++;
        const char *errstr;
        int erroffset = 0;
        current_info.template = pcre_compile(current_info.template_char, 0,
                                             &errstr, &erroffset, NULL);
      } else {
        parser_flags(&current_info, i, argv, flags_count);
      }
    } else if (strstr(argv[i], ".txt") != NULL ||
               strstr(argv[i], ".file") != NULL ||
               strstr(argv[i], ".bin") != NULL) {
      strcpy(current_info.file_name[*files_count], argv[i]);
      *files_count += 1;
    } else {
      if (templates_count == 0) {
        strcpy(current_info.template_char, argv[i]);
        const char *errstr;
        int erroffset = 0;
        if (current_info.opt_i != 0) {
          current_info.template =
              pcre_compile(argv[i], PCRE_CASELESS, &errstr, &erroffset, NULL);
          templates_count++;
        } else {
          current_info.template =
              pcre_compile(argv[i], 0, &errstr, &erroffset, NULL);
          templates_count++;
        }
      } else if (current_info.opt_s == 0) {
        strcpy(current_info.file_name[*files_count], argv[i]);
        *files_count += 1;
      }
    }
  }
  if (templates_count == 0 || *flags_count > 2) {
    current_info.errors++;
  }
  return current_info;
}

void parser_flags(struct s_info *current_info, int i, char **argv,
                  int *flags_count) {
  for (int j = 1; j < (int)(strlen(argv[i])); j++) {
    switch (argv[i][j]) {
      case 'e':
        if (current_info->opt_e == 0) {
          current_info->opt_e = j;
          *flags_count += 1;
        }
        break;
      case 'i':
        if (current_info->opt_i == 0) {
          current_info->opt_i = j;
          *flags_count += 1;
        }
        break;
      case 'v':
        if (current_info->opt_v == 0) {
          current_info->opt_v = j;
          *flags_count += 1;
        }
        break;
      case 'c':
        if (current_info->opt_c == 0) {
          current_info->opt_c = j;
          *flags_count += 1;
        }
        break;
      case 'l':
        if (current_info->opt_l == 0) {
          current_info->opt_l = j;
          *flags_count += 1;
        }
        break;
      case 'n':
        if (current_info->opt_n == 0) {
          current_info->opt_n = j;
          *flags_count += 1;
        }
        break;
      case 'h':
        if (current_info->opt_h == 0) {
          current_info->opt_h = j;
          *flags_count += 1;
        }
        break;
      case 's':
        if (current_info->opt_s == 0) {
          current_info->opt_s = j;
          *flags_count += 1;
        }
        break;
      case 'o':
        if (current_info->opt_o == 0) {
          current_info->opt_o = j;
          *flags_count += 1;
        }
        break;
      default:
        current_info->errors++;
        *flags_count += 1;
        break;
    }
  }
}

void grep(struct s_info current_info, int files_count, int flags_count,
          int e_status) {
  for (int i = 0; i < files_count; i++) {
    FILE *file = NULL;
    if ((file = fopen(current_info.file_name[i], "r")) != NULL) {
      int c_counter = 0;
      int n_counter = 0;
      int l_status = 0;
      while (!feof(file)) {
        char string[MAX] = {0};
        fgets(string, MAX, file);
        int str_status = 0;
        if (current_info.opt_e != 0 || flags_count == 0) {
          option_e(&current_info, i, files_count, &flags_count, string,
                   e_status);
        }
        if (current_info.opt_i != 0) {
          option_i(current_info, i, files_count, flags_count, string,
                   &str_status);
        }
        if (current_info.opt_v != 0) {
          option_v(current_info, i, files_count, flags_count, string,
                   str_status);
        }
        if (current_info.opt_c != 0) {
          option_c1(current_info, &c_counter, string, str_status);
        }
        if (current_info.opt_s != 0) {
          option_s(current_info, i, files_count, flags_count, string,
                   str_status);
        }
        if (current_info.opt_l != 0) {
          option_l(current_info, string, str_status, &l_status);
        }
        if (current_info.opt_h != 0) {
          option_h(current_info, flags_count, string);
        }
        if (current_info.opt_n != 0) {
          option_n(current_info, i, string, &n_counter, str_status,
                   files_count);
        }
        if (current_info.opt_o != 0) {
          option_o(current_info, flags_count, string, str_status, files_count,
                   i);
        }
      }
      rewind(file);
      if (current_info.opt_c != 0 && current_info.opt_l == 0) {
        option_c2(current_info, i, c_counter, flags_count, files_count);
      }
      if (l_status != 0 && getc(file) != EOF) {
        printf("%s\n", current_info.file_name[i]);
      }
      fclose(file);
    } else {
      if (current_info.opt_s == 0)
        fprintf(stderr, "grep: %s: Нет такого файла или каталога\n",
                current_info.file_name[i]);
    }
  }
}

void option_e(struct s_info *current_info, int file_number, int files_count,
              int *flags_count, char *string, int e_status) {
  if (*flags_count == 1 || *flags_count == 0 ||
      (*flags_count == 2 && current_info->opt_e == 1)) {
    int ovector[MAX];
    if (pcre_exec(current_info->template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0) {
      if (files_count > 1 || e_status == 1) {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s:%s\n", current_info->file_name[file_number], string);
        } else {
          printf("%s:%s", current_info->file_name[file_number], string);
        }
      } else {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s\n", string);
        } else {
          printf("%s", string);
        }
      }
    }
  } else {
    current_info->opt_e = 0;
    *flags_count -= 1;
  }
}

void option_i(struct s_info current_info, int file_number, int files_count,
              int flags_count, char *string, int *str_status) {
  int ovector[MAX];
  if (flags_count == 1 || (flags_count == 2 && (current_info.opt_h != 0 ||
                                                current_info.opt_s != 0))) {
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0) {
      if (files_count == 1 || current_info.opt_h != 0) {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s\n", string);
        } else {
          printf("%s", string);
        }
      } else {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s:%s\n", current_info.file_name[file_number], string);
        } else {
          printf("%s:%s", current_info.file_name[file_number], string);
        }
      }
    }
  } else {
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0) {
      *str_status += 1;
    }
  }
}

void option_v(struct s_info current_info, int file_number, int files_count,
              int flags_count, char *string, int str_status) {
  if (flags_count == 1 || (flags_count == 2 && (current_info.opt_h != 0 ||
                                                current_info.opt_i != 0 ||
                                                current_info.opt_s != 0))) {
    int ovector[MAX];
    if ((current_info.opt_i == 0 &&
         (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                    0, ovector, MAX) == -1)) ||
        (current_info.opt_i != 0 && str_status == 0)) {
      if (files_count == 1 || current_info.opt_h != 0) {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s\n", string);
        } else {
          printf("%s", string);
        }
      } else {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s:%s\n", current_info.file_name[file_number], string);
        } else if (string[strlen(string) - 1] != '\n' && string[0] == '\0') {
          printf("%s", string);
        } else {
          printf("%s:%s", current_info.file_name[file_number], string);
        }
      }
    }
  }
}

void option_c1(struct s_info current_info, int *c_counter, char *string,
               int str_status) {
  int ovector[MAX];
  if (current_info.opt_v == 0) {
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0 ||
        str_status != 0) {
      *c_counter += 1;
    }
  } else {
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) == -1) {
      if (string[0] != '\0') {
        *c_counter += 1;
      }
    }
  }
}

void option_c2(struct s_info current_info, int file_number, int c_counter,
               int flags_count, int files_count) {
  if (flags_count == 1) {
    if (files_count > 1) {
      printf("%s:%d\n", current_info.file_name[file_number], c_counter);
    } else {
      printf("%d\n", c_counter);
    }
  } else {
    if (current_info.opt_h == 0 && files_count > 1) {
      printf("%s:%d\n", current_info.file_name[file_number], c_counter);
    } else {
      printf("%d\n", c_counter);
    }
  }
}

void option_s(struct s_info current_info, int file_number, int files_count,
              int flags_count, char *string, int str_status) {
  if (flags_count == 1 || (flags_count == 2 && current_info.opt_h != 0)) {
    int ovector[MAX];
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0 ||
        str_status != 0) {
      if (files_count == 1 || (flags_count == 2 && current_info.opt_h != 0)) {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s\n", string);
        } else {
          printf("%s", string);
        }
      } else {
        if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
          printf("%s:%s\n", current_info.file_name[file_number], string);
        } else {
          printf("%s:%s", current_info.file_name[file_number], string);
        }
      }
    }
  }
}

void option_l(struct s_info current_info, char *string, int str_status,
              int *l_status) {
  int ovector[MAX];
  if (current_info.opt_v == 0) {
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0 ||
        (current_info.opt_i != 0 && str_status != 0)) {
      *l_status += 1;
    }
  } else {
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) == -1) {
      *l_status += 1;
    }
  }
}

void option_n(struct s_info current_info, int file_number, char *string,
              int *n_counter, int str_status, int files_count) {
  *n_counter += 1;
  if (current_info.opt_c == 0 && current_info.opt_l == 0) {
    int ovector[MAX];
    if (current_info.opt_v == 0) {
      if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                    0, ovector, MAX) > 0 ||
          (current_info.opt_i != 0 && str_status != 0)) {
        if (current_info.opt_h != 0 ||
            (current_info.opt_o == 0 && files_count == 1)) {
          if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
            printf("%d:%s\n", *n_counter, string);
          } else {
            printf("%d:%s", *n_counter, string);
          }
        } else if (current_info.opt_o != 0) {
          if (files_count > 1) {
            printf("%s:%d:%s\n", current_info.file_name[file_number],
                   *n_counter, current_info.template_char);
          } else {
            printf("%d:%s\n", *n_counter, current_info.template_char);
          }
        } else {
          if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
            printf("%s:%d:%s\n", current_info.file_name[file_number],
                   *n_counter, string);
          } else {
            printf("%s:%d:%s", current_info.file_name[file_number], *n_counter,
                   string);
          }
        }
      }
    } else {
      if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                    0, ovector, MAX) == -1 ||
          (current_info.opt_i != 0 && str_status == 0)) {
        if (files_count == 1) {
          if (string[strlen(string) - 1] != '\n') {
            if (string[0] != '\0') {
              printf("%d:%s\n", *n_counter, string);
            }
          } else {
            printf("%d:%s", *n_counter, string);
          }
        } else {
          if (string[strlen(string) - 1] != '\n') {
            if (string[0] != '\0') {
              printf("%s:%d:%s\n", current_info.file_name[file_number],
                     *n_counter, string);
            }
          } else {
            printf("%s:%d:%s", current_info.file_name[file_number], *n_counter,
                   string);
          }
        }
      }
    }
  }
}

void option_h(struct s_info current_info, int flags_count, char *string) {
  int ovector[MAX];
  if (flags_count == 1 &&
      pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0, 0,
                ovector, MAX) > 0) {
    if (string[strlen(string) - 1] != '\n' && string[0] != '\0') {
      printf("%s\n", string);
    } else {
      printf("%s", string);
    }
  }
}

void option_o(struct s_info current_info, int flags_count, char *string,
              int str_status, int files_count, int file_number) {
  if (flags_count == 1 || (flags_count == 2 && (current_info.opt_h != 0 ||
                                                current_info.opt_s != 0 ||
                                                current_info.opt_i != 0))) {
    int ovector[MAX];
    if (pcre_exec(current_info.template, NULL, string, (int)strlen(string), 0,
                  0, ovector, MAX) > 0 ||
        (current_info.opt_i != 0 && str_status != 0)) {
      if (files_count == 1 || current_info.opt_h != 0) {
        printf("%s\n", current_info.template_char);
      } else {
        printf("%s:%s\n", current_info.file_name[file_number],
               current_info.template_char);
      }
    }
  }
}
