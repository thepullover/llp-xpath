#include "structure.h"

size_t size = 0;

void *pr_malloc(size_t size_of){
    size += size_of;
    return malloc(size_of);
}

void *print_memory(){
    printf("%zu\n", size);
}

struct view *create_view(enum crud crud_op){
    struct view *view = pr_malloc(sizeof(struct view));
    view->tree_lvl = NULL;
    view->crud_operation = crud_op;
    return view;
}

struct list_level *create_list_level(uint8_t negative, uint8_t any, enum parent place){
    struct list_level *list_level = pr_malloc(sizeof(struct list_level));
    list_level->next = NULL;
    list_level->value = NULL;
    list_level->place = place;
    list_level->any = any;
    list_level->negative = negative;
    list_level->filters = NULL;
    return list_level;
}

struct list_element *create_list_element(int64_t id){
    struct list_element *list_element = pr_malloc(sizeof(struct list_element));
    list_element->next = NULL;
    list_element->element = id;
    return list_element;
}

struct filter_list *create_filter_list(){
    struct filter_list *filter_list = pr_malloc(sizeof(struct filter_list));
    filter_list->next = NULL;
    filter_list->value = NULL;
    return filter_list;
}

struct filter *create_filter(uint8_t negative){
    struct filter *filter = pr_malloc(sizeof(struct filter));
    filter->negative = negative;
    filter->comparators = NULL;
    return filter;
}

struct comparator_list *create_comparator_list(){
    struct comparator_list *comparator_list = pr_malloc(sizeof(struct comparator_list));
    comparator_list->value = NULL;
    comparator_list->next = NULL;
    return comparator_list;
}

struct comparator *create_comparator(){
    struct comparator *comparator = pr_malloc(sizeof(struct comparator));
    comparator->negative = 0;
    comparator->true = 0;
    comparator->operation = 0;
    comparator->operation_1 = NULL;
    comparator->operation_2 = NULL;
    return comparator;
}

struct operator *create_operator(uint8_t field, enum field_types type, union types value){
    struct operator *operator = pr_malloc(sizeof(struct operator));
    operator->value = value;
    operator->type = type;
    operator->field = field;
    return operator;
}

struct field *create_field(size_t size, char *value){
    struct field* field = pr_malloc(sizeof(struct field));
    field->value = value;
    field->size = size;
    return field;
}
