#include <keyboardBinds.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SEARCH_MACRO(total_keys, standart_keys, key_old) \
    for (int i = 0; i < total_keys; i++) { \
        if (strcmp(key_old, standart_keys[i].name) == 0) { \
            printf("Found: %s has code %d\n", standart_keys[i].name, standart_keys[i].value); \
            break; \
        } \
    }

void readConfFile(FILE *file) {
    struct MacroMap standart_keys[] = {
#define X(name) { #name, name },
        MY_MACRO_LIST
#undef X
    };
        
    const int total_keys = sizeof(standart_keys) / sizeof(standart_keys[0]);

    char line[MAX_LINE];
    struct KeyBind binds_array[MAX_LINE];
    size_t barray_index = 0;
    char key_old[20], key_new[20];
    int key_old_value = -1;
    // const int total_keys = sizeof(standart_keys) / sizeof(standart_keys[0]);

    while (fscanf(file, "%49s %49s", key_old, key_new) == 2) {
        // printf("%s %s\n", key_old, key_new);
        // SEARCH_MACRO(total_keys, standart_keys, key_old)
        // SEARCH_MACRO(total_keys, standart_keys, key_new)
        for (int i = 0; i < total_keys; i++) {
            if (strcmp(key_old, standart_keys[i].name) == 0) {
                key_old_value = standart_keys[i].value;
                // printf("Found: %s has code %d\n", standart_keys[i].name, standart_keys[i].value);
                break;
            }
        }
        if (key_old_value == -1) {
            continue;
        }

        for (int i = 0; i < total_keys; i++) {
            if (strcmp(key_new, standart_keys[i].name) == 0) {
                binds_array[barray_index].newkey = standart_keys[i].value;
                binds_array[barray_index].oldkey = key_old_value;
                barray_index++; // *barray_index += 1;
                // printf("Found: %s has code %d\n", standart_keys[i].name, standart_keys[i].value);
                break;
            }
        }
        
    }
    // printf("Uploaded configurations\n");
    // for (int i = 0; i < barray_index; i++) {
    //     printf(" %d instead of %d\n", binds_array[i].newkey, binds_array[i].oldkey);
    // }
}

void parseLine(char line[MAX_LINE]) {
    char *word = strtok(line, " \t");
    while (word) {
        printf("  Слово: %s\n", word);
        word = strtok(NULL, " \t");
    }
}

void returnCode(struct input_event *ev) {
    if (ev->code == KEY_W) {
        ev->code = KEY_Q;
    }
}
