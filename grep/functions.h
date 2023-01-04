#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <pcre.h>
#include <stdio.h>

struct s_info {
  char **file_name;
  char *template_char;
  pcre *template;
  int opt_e;
  int opt_i;
  int opt_v;
  int opt_c;
  int opt_l;
  int opt_n;
  int opt_h;
  int opt_s;
  int opt_o;
  int errors;
};

struct s_info parser(int argc, char **argv, int *files_count, int *flags_count,
                     int e_status);
void parser_flags(struct s_info *current_info, int i, char **argv,
                  int *flags_count);
void grep(struct s_info current_info, int files_count, int flags_count,
          int e_status);
void option_e(struct s_info *current_info, int file_number, int files_count,
              int *flags_count, char *string, int e_status);
void option_i(struct s_info current_info, int file_number, int files_count,
              int flags_count, char *string, int *str_status);
void option_v(struct s_info current_info, int file_number, int files_count,
              int flags_count, char *string, int str_status);
void option_c1(struct s_info current_info, int *c_counter, char *string,
               int str_status);
void option_c2(struct s_info current_info, int file_number, int c_counter,
               int flags_count, int files_count);
void option_l(struct s_info current_info, char *string, int str_status,
              int *l_status);
void option_n(struct s_info current_info, int file_number, char *string,
              int *n_counter, int str_status, int files_count);
void option_h(struct s_info current_info, int flags_count, char *string);
void option_o(struct s_info current_info, int flags_count, char *string,
              int str_status, int files_count, int file_number);
void option_s(struct s_info current_info, int file_number, int files_count,
              int flags_count, char *string, int str_status);

#endif  // FUNCTIONS_H