#include "parser.h"

enum states for_next(char **string, struct list_level *list_lvl) {
    if (**string == '/' && *(*string)++)
        return STATE_NAME;
    return STATE_ERROR;
};

enum states for_name(char **string, struct list_level *level) {
    uint8_t negative = 0,val = 0;
    if (**string == '!' && *(*string)++)
        negative = 1;
    if (**string == '*' && *(*string)++)
        val = 1;
    if (!val) {
        if (**string && **string > '0' && **string < '9') {
            char *temp_p = *string; size_t len = 0;
            while (temp_p[len] >= '0' && temp_p[len] <= '9') len++;
            temp_p = *string; int64_t id = 0;
            for (size_t iter = 0; iter < len; iter++) {
                id = 10 * id + temp_p[iter] - '0';
            }
            level->value = create_list_element(id); level->negative = negative; *string = temp_p + len;
        } else {
            return STATE_ERROR;
        }
    } else {
        level->any = 1;
    }
    if (**string == '[' || **string == '!') {
        return STATE_ATTRIBUTE;
    } else {
        return STATE_NEXT;
    }

};

struct operator *read_operator(char **string){
    uint8_t number = 0;
    if (**string && **string > '0' && **string < '9') number = 1;
    char *temp_p = *string; size_t len = 0; struct operator *op;
    if (number) {
        double value = 0; uint8_t i = 0;
        while (temp_p[len] && (temp_p[len] >= '0' && temp_p[len] <= '9' || temp_p[len] == '.')) {
            if (temp_p[len] == '.') {
                len++;
                if (i) return NULL;
                i = 1;
                continue;
            }
            if (i) {
                double exp = 0.1;
                for(size_t iter = 1; iter < i; iter++) exp *= 0.1;
                i++;
                value = value +  exp * (temp_p[len] - '0');
            } else {
                value = 10 * value + temp_p[len] - '0';
            }
            len++;
        }
        union types t;
        if (i) {
            t.real = value; op = create_operator(!number, FLOAT_TYPE, t);
        } else {
            t.integer = (int64_t) value; op = create_operator(!number, INTEGER_TYPE, t);
        }
    } else {
        uint8_t field = 1;
        if (**string == '\'' && *(*string)++) field = 0; temp_p = *string;
        while (temp_p[len] && (temp_p[len] >= 'a' && temp_p[len] <= 'z' || temp_p[len] >= 'A' && temp_p[len] <= 'Z')){
            len++;
        }
        char *value = pr_malloc(sizeof(char) * len);
        temp_p = *string;
        for (size_t iter = 0; iter < len; iter++){
            value[iter] = temp_p[iter];
        }
        struct field *f = create_field(len, value);
        union types t;
        t.string = f;
        op = create_operator(!number, STRING_TYPE, t); op->field = field;
    }
    *string += len;
    return op;
}

struct filter_list *create_wrap_filter_list(uint8_t negative){
    struct filter *f = create_filter(negative);
    struct filter_list *fl = create_filter_list();
    fl->value = f;
    return fl;
}

struct comparator *read_filter(char **string){
    struct comparator *comp = create_comparator();
    if (**string && **string=='!' && *(*string)++) comp->negative = 1;
    if (**string && **string=='@' && *(*string)++) comp->true = 1;
    else {
        comp->operation_1 = read_operator(string);
        comp->operation = (enum compare) *(*string)++;
        comp->operation_2 = read_operator(string);
    }
    return comp;
}

void append_comparator(struct filter_list *fl, struct comparator *comp){
    struct comparator_list *cl = create_comparator_list();
    cl->value = comp; cl->next = fl->value->comparators;
    fl->value->comparators = cl;
}

enum states for_attribute(char **string, struct list_level *level) {
    uint8_t negative = 0;
    struct comparator *comp;
    if (**string && **string=='!' && *(*string)++) negative++;
    if (**string =='['){
        struct filter_list *fl = create_wrap_filter_list(negative);
        fl->next = level->filters;
        level->filters = fl;
        while(**string != ']' && *(*string)++) {
            comp = read_filter(string);
            append_comparator(level->filters, comp);
            if (**string != '|' && **string != ']' && *(*string)++) return STATE_ERROR;
        } (*string)++;
        if (**string == '[' || **string == '!') return STATE_ATTRIBUTE;
        else return STATE_NEXT;
    } else {
        return STATE_ERROR;
    }

};

enum states for_error(char **string, struct list_level *level) {
    return STATE_ERROR;
};

enum states (*op_table[4])(char **, struct list_level *) = {
        [STATE_NEXT] = for_next,
        [STATE_NAME] = for_name,
        [STATE_ATTRIBUTE] = for_attribute,
        [STATE_ERROR] = for_error
};

struct view *parse_operation(char op_char){
    if (op_char != '-' && op_char != '+' && op_char != '?' && op_char != '=') return NULL;
    return create_view(op_char);
}

enum states parse_state(enum states cur_state, char **string, struct list_level *level){
    return op_table[cur_state](string, level);
}

