#include "parser.h"
#include "io_handler.h"

#define BUFF_COMMAND 2048

/*
 * тестовая программа принимает запрос и выводит результат его обработки
 */

int main(){
    char command[BUFF_COMMAND], *com;
    enum states c_state;
    scanf("%s", command);
    com = command + 1;
    struct view *view = parse_operation(command[0]);
    struct list_level *cur_level = create_list_level(0, 0 , PARENT_ROOT);
    if (*com == '/'){
        c_state = STATE_NEXT;
    } else if (*com){
        cur_level->place = PARENT_FREE;
        command[0] = '/';
        com = command;
        c_state = STATE_NEXT;
    } else {
        com = command;
        c_state = STATE_ERROR;
    }
    while (com && c_state != STATE_ERROR) {
        c_state = parse_state(c_state, &com, cur_level);
        if (c_state == STATE_NEXT && *com) {
            cur_level->next = view->tree_lvl;
            view->tree_lvl = cur_level;
            cur_level = create_list_level(0, 0, PARENT_RELATIVE);
        }
    }
    if (cur_level) {
        cur_level->next = view->tree_lvl;
        view->tree_lvl = cur_level;
    }
    print_output(c_state, com, view);
    printf("\n memory usage: ");
    print_memory();
    return 0;
}