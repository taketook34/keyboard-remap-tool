#include <keyboardBinds.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void readConfFile(FILE *file, struct KeyBind *binds_array, size_t *barray_index) {
    struct MacroMap standart_keys[] = {
#define X(name) { #name, name },
        MY_MACRO_LIST
#undef X
    };
        
    const int total_keys = sizeof(standart_keys) / sizeof(standart_keys[0]);
    char line[MAX_LINE];
    char key_old[20], key_new[20];
    int key_old_value = -1;

    while (fscanf(file, "%49s %49s", key_old, key_new) == 2) {
        for (int i = 0; i < total_keys; i++) {
            if (strcmp(key_old, standart_keys[i].name) == 0) {
                key_old_value = standart_keys[i].value;
                break;
            }
        }
        if (key_old_value == -1) {
            continue;
        }

        for (int i = 0; i < total_keys; i++) {
            if (strcmp(key_new, standart_keys[i].name) == 0) {
                binds_array[*barray_index].newkey = standart_keys[i].value;
                binds_array[*barray_index].oldkey = key_old_value;
                *barray_index += 1; // barray_index++;
                break;
            }
        }  
    }
}


void returnCode(struct input_event *ev,  struct KeyBind *binds_array, size_t barray_index) {
    for (int i = 0; i < barray_index; i++) {
        if (ev->code == binds_array[i].oldkey) {
            // printf("Found value %d\n", ev->code);
            ev->code = binds_array[i].newkey;
            break;
        }
    }
}
