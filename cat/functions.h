#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct s_info {
  char** file_name;
  int errors;
  int opt_b;
  int opt_n;
  int opt_s;
  int opt_e;
  int opt_t;
  int opt_E;
  int opt_T;
};

struct s_info parser(int argc, char** argv, int* file_count);

void flags_processing(struct s_info* current_info, char** flags_arr,
                      int flags_count);

void concatenate(struct s_info current_info, int files_count, int* str_counter,
                 int* is_not_first_file);

void print_text(struct s_info current_info, char** text, int lines_count,
                int* str_counter, int is_first_file);

void option_s(char** text, int lines_count);

void option_t(char** text, int lines_count);

void option_e(char** text, int lines_count);

void option_T(char** text, int lines_count);

void option_E(char** text, int lines_count);

#endif  // FUNCTIONS_H