#ifndef LAB2_LLP_PARSER_H
#define LAB2_LLP_PARSER_H
#include "structure.h"

struct view *parse_operation(char op_char);
enum states parse_state(enum states cur_state, char **string, struct list_level *level);
#endif //LAB2_LLP_PARSER_H
