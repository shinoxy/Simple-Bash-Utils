#ifndef SRC_CAT_FUNCTIONS_H
#define SRC_CAT_FUNCTIONS_H

struct s_info {
  char **file_name;
  char *option;
};

struct s_info parser(int argc, char **argv, int *file_count,
                     int *does_flag_exists);

void flags_processing(struct s_info *current_info, char **flags_arr,
                      int flags_count);

void concatenate(struct s_info current_info, int files_count,
                 int does_flag_exists);

void option_s(char **text, int lines_count);

void option_t(char **text, int lines_count);

void option_e(char **text, int lines_count);

void option_T(char **text, int lines_count);

void option_E(char **text, int lines_count);

void option_b(char **text, int lines_count, int *str_count);

void option_n(char **text, int lines_count, int *str_count);

#endif  // SRC_CAT_FUNCTIONS_H